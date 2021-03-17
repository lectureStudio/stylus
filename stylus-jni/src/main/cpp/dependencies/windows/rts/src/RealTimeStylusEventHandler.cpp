/*
 * Copyright 2016 Alex Andres
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RealTimeStylusEventHandler.h"
#include "RealTimeStylusUtils.h"

#include <rtscom_i.c>
#include <sstream>
#include <unordered_map>
#include <Windowsx.h>

namespace stylus
{
	std::unordered_map<HWND, RealTimeStylusEventHandler *> windowMap;


	RealTimeStylusEventHandler::RealTimeStylusEventHandler(HWND hWnd, EventDispatcher<StylusEvent> * eventDispatcher) :
		hWnd(hWnd),
		hookHandle(nullptr),
		eventDispatcher(eventDispatcher),
		refCount(1),
		marshaller(nullptr),
		realTimeStylus(nullptr),
		button(Button::NONE),
		stylusContext(nullptr),
		dpiScaleX(0),
		dpiScaleY(0),
		stylusActive(false)
	{
		initialize();
	}

	RealTimeStylusEventHandler::~RealTimeStylusEventHandler() {
		if (realTimeStylus != nullptr) {
			realTimeStylus->Release();
		}
		if (marshaller != nullptr) {
			marshaller->Release();
		}
		if (hookHandle != nullptr) {
			if (!UnhookWindowsHookEx(hookHandle)) {
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()), "Unhook window failed");
			}
		}
	}

	HRESULT RealTimeStylusEventHandler::DataInterest(RealTimeStylusDataInterest * pEventInterest)
	{
		long interest = RTSDI_Packets;
		interest |= RTSDI_InAirPackets;
		interest |= RTSDI_StylusButtonDown;
		interest |= RTSDI_StylusButtonUp;
		interest |= RTSDI_StylusDown;
		interest |= RTSDI_StylusInRange;
		interest |= RTSDI_StylusOutOfRange;

		*pEventInterest = static_cast<RealTimeStylusDataInterest>(interest);

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::Packets(IRealTimeStylus * pStylus, const StylusInfo * pStylusInfo, ULONG nPackets, ULONG nPacketBuf, LONG * pPackets, ULONG * nOutPackets, LONG ** ppOutPackets)
	{
		createEvent(EventType::MOVE, pStylusInfo, nPackets, nPacketBuf, pPackets);

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::InAirPackets(IRealTimeStylus * pStylus, const StylusInfo * pStylusInfo, ULONG nPackets, ULONG nPacketBuf, LONG * pPackets, ULONG * nOutPackets, LONG ** ppOutPackets)
	{
		RealTimeStylusCursorContext * context = getContext(pStylusInfo->cid);

		stylusContext = context;

		createEvent(EventType::MOVE, pStylusInfo, nPackets, nPacketBuf, pPackets);

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::StylusButtonDown(IRealTimeStylus * piRtsSrc, STYLUS_ID sid, const GUID * pGuidStylusButton, POINT * point)
	{
		if (!stylusActive) {
			return S_OK;
		}

		RealTimeStylusCursorContext * context = getContext(sid);
		Button evButton;
		Cursor evCursor;

		if (context == nullptr) {
			// For compatibility reasons. Some tablets do not send all button events.
			evCursor = Cursor::PEN;

			if (GetKeyState(VK_LBUTTON) < 0) {
				evButton = Button::LEFT;
			}
			else if (GetKeyState(VK_RBUTTON) < 0) {
				evButton = Button::RIGHT;
			}
			else if (GetKeyState(VK_MBUTTON) < 0) {
				evButton = Button::MIDDLE;
			}
			else {
				evButton = Button::NONE;
			}
		}
		else {
			evButton = context->getButton(pGuidStylusButton);
			evCursor = context->cursor;
		}

		// Since the packet data is in Ink Space coordinates, we need to convert to pixels.
		double x = point->x * dpiScaleX;
		double y = point->y * dpiScaleY;

		button = evButton;

		StylusAxesData axesData;
		axesData[Axis::X] = x;
		axesData[Axis::Y] = y;

		auto event = std::make_shared<StylusEvent>(EventType::BUTTON_DOWN, evButton, evCursor, std::move(axesData));
		eventDispatcher->dispatchEvent(event);

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::StylusButtonUp(IRealTimeStylus * piRtsSrc, STYLUS_ID sid, const GUID * pGuidStylusButton, POINT * point)
	{
		if (!stylusActive) {
			return S_OK;
		}

		RealTimeStylusCursorContext * context = getContext(sid);

		if (context == nullptr) {
			return S_OK;
		}

		// Since the packet data is in Ink Space coordinates, we need to convert to pixels.
		double x = point->x * dpiScaleX;
		double y = point->y * dpiScaleY;

		Button cursorButton = context->getButton(pGuidStylusButton);

		StylusAxesData axesData;
		axesData[Axis::X] = x;
		axesData[Axis::Y] = y;

		auto event = std::make_shared<StylusEvent>(EventType::BUTTON_UP, cursorButton, context->cursor, std::move(axesData));
		eventDispatcher->dispatchEvent(event);

		button = Button::NONE;

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::StylusInRange(IRealTimeStylus * rtsSrc, TABLET_CONTEXT_ID, STYLUS_ID sid)
	{
		RealTimeStylusCursorContext * context = getContext(sid);

		if (context == nullptr) {
			return S_OK;
		}

		stylusActive = true;
		stylusContext = context;

		StylusAxesData axesData;

		auto event = std::make_shared<StylusEvent>(EventType::CURSOR, button, context->cursor, std::move(axesData));
		eventDispatcher->dispatchEvent(event);

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::StylusOutOfRange(IRealTimeStylus * rtsSrc, TABLET_CONTEXT_ID, STYLUS_ID sid)
	{
		RealTimeStylusCursorContext * context = getContext(sid);

		if (context == nullptr) {
			return S_OK;
		}

		stylusActive = false;
		stylusContext = nullptr;

		StylusAxesData axesData;

		auto event = std::make_shared<StylusEvent>(EventType::CURSOR, button, context->cursor, std::move(axesData));
		eventDispatcher->dispatchEvent(event);

		return S_OK;
	}

	HRESULT RealTimeStylusEventHandler::StylusDown(IRealTimeStylus*, const StylusInfo * pStylusInfo, ULONG cPropCountPerPkt, LONG * pPackets, LONG**)
	{
		RealTimeStylusCursorContext * context = getContext(pStylusInfo->cid);

		if (context == nullptr) {
			// Mouse button down.
			stylusContext = nullptr;

			return S_OK;
		}

		stylusContext = context;

		return S_OK;
	}

	// Unused methods.
	HRESULT RealTimeStylusEventHandler::TabletAdded(IRealTimeStylus*, IInkTablet*) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::TabletRemoved(IRealTimeStylus*, LONG) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::StylusUp(IRealTimeStylus*, const StylusInfo*, ULONG, LONG*, LONG**) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::RealTimeStylusEnabled(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::RealTimeStylusDisabled(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::SystemEvent(IRealTimeStylus*, TABLET_CONTEXT_ID, STYLUS_ID, SYSTEM_EVENT, SYSTEM_EVENT_DATA) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::CustomStylusDataAdded(IRealTimeStylus*, const GUID*, ULONG, const BYTE*) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::Error(IRealTimeStylus*, IStylusPlugin*, RealTimeStylusDataInterest, HRESULT, LONG_PTR*) { return S_OK; }
	HRESULT RealTimeStylusEventHandler::UpdateMapping(IRealTimeStylus*) { return S_OK; }

	// IUnknown methods.
	ULONG RealTimeStylusEventHandler::AddRef()
	{
		return InterlockedIncrement(&refCount);
	}

	ULONG RealTimeStylusEventHandler::Release()
	{
		ULONG nNewRef = InterlockedDecrement(&refCount);

		if (nNewRef == 0) {
			delete this;
		}

		return nNewRef;
	}

	HRESULT RealTimeStylusEventHandler::QueryInterface(REFIID riid, LPVOID * ppvObj)
	{
		if ((riid == IID_IStylusSyncPlugin) || (riid == IID_IUnknown)) {
			*ppvObj = this;
			AddRef();
			return S_OK;
		}
		else if ((riid == IID_IMarshal) && (marshaller != NULL)) {
			return marshaller->QueryInterface(riid, ppvObj);
		}

		*ppvObj = NULL;

		return E_NOINTERFACE;
	}

	void RealTimeStylusEventHandler::initialize()
	{
		HRESULT hr = CoCreateInstance(CLSID_RealTimeStylus, NULL, CLSCTX_ALL, IID_PPV_ARGS(&realTimeStylus));
		if (FAILED(hr)) {
			ThrowIfFailed(hr, "Create RealTimeStylus failed");
		}

		hr = realTimeStylus->put_HWND(reinterpret_cast<HANDLE_PTR>(hWnd));
		if (FAILED(hr)) {
			realTimeStylus->Release();
			ThrowIfFailed(hr, "Attach RealTimeStylus to a window failed");
		}

		// Register RTS object for receiving multi-touch input.
		IRealTimeStylus3 * realTimeStylus3 = nullptr;
		hr = realTimeStylus->QueryInterface(&realTimeStylus3);

		if (SUCCEEDED(hr)) {
			hr = realTimeStylus3->put_MultiTouchEnabled(TRUE);

			if (FAILED(hr)) {
				printf("RealTimeStylus: Failed to enable multi-touch\n");
			}

			realTimeStylus3->Release();
		}

		hr = CoCreateFreeThreadedMarshaler(this, &marshaller);
		if (FAILED(hr)) {
			realTimeStylus->Release();
			ThrowIfFailed(hr, "Create free-threaded marshaller for the RealTimeStylus failed");
		}

		hr = realTimeStylus->AddStylusSyncPlugin(0, this);
		if (FAILED(hr)) {
			realTimeStylus->Release();
			ThrowIfFailed(hr, "Add handler to the list of synchronous plugins in the RealTimeStylus failed");
		}

		GUID wantedProps[] = {
			GUID_PACKETPROPERTY_GUID_X,
			GUID_PACKETPROPERTY_GUID_Y,
			GUID_PACKETPROPERTY_GUID_NORMAL_PRESSURE,
			GUID_PACKETPROPERTY_GUID_TANGENT_PRESSURE,
			GUID_PACKETPROPERTY_GUID_X_TILT_ORIENTATION,
			GUID_PACKETPROPERTY_GUID_Y_TILT_ORIENTATION
		};
		
		realTimeStylus->SetDesiredPacketDescription(6, wantedProps);
		realTimeStylus->put_Enabled(true);

		// Detect what cursors will give us pressure data.
		initializeCursors();

		initializeWindowHook();

		// Get screen DPI scale.
		HDC hdc = GetDC(hWnd);
		dpiScaleX = GetDeviceCaps(hdc, LOGPIXELSX) / 2540.0F;
		dpiScaleY = GetDeviceCaps(hdc, LOGPIXELSY) / 2540.0F;
		ReleaseDC(hWnd, hdc);
	}

	void RealTimeStylusEventHandler::initializeWindowHook()
	{
		// Hook into the window.
		DWORD threadId = GetWindowThreadProcessId(hWnd, nullptr);
		HINSTANCE appInstance = GetModuleHandle(NULL);

		hookHandle = SetWindowsHookEx(WH_GETMESSAGE, windowProc, appInstance, threadId);

		if (hookHandle == nullptr) {
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()), "Set window hook failed");
		}

		windowMap[hWnd] = this;
	}

	void RealTimeStylusEventHandler::initializeCursors()
	{
		IInkCursors * inkCursors;

		HRESULT hr = realTimeStylus->GetStyluses(&inkCursors);
		if (FAILED(hr)) {
			ThrowIfFailed(hr, "Get styluses failed");
		}

		long cursorCount;

		hr = inkCursors->get_Count(&cursorCount);
		if (FAILED(hr)) {
			ThrowIfFailed(hr, "Get cursor count failed");
		}

		for (long i = 0; i < cursorCount; i++) {
			IInkCursor * inkCursor;
			
			hr = inkCursors->Item(i, &inkCursor);
			if (FAILED(hr)) {
				continue;
			}

			initializeCursor(inkCursor);
		}
	}

	void RealTimeStylusEventHandler::initializeCursor(IInkCursor * inkCursor)
	{
		LONG sid;

		HRESULT hr = inkCursor->get_Id(&sid);
		if (FAILED(hr)) {
			return;
		}

		IInkTablet * inkTablet;

		hr = inkCursor->get_Tablet(&inkTablet);
		if (FAILED(hr)) {
			return;
		}

		TABLET_CONTEXT_ID contextId;

		hr = realTimeStylus->GetTabletContextIdFromTablet(inkTablet, &contextId);
		if (FAILED(hr)) {
			return;
		}

		float scaleX, scaleY;
		ULONG packetPropsCount = 0;
		PACKET_PROPERTY * packetProps;

		realTimeStylus->GetPacketDescriptionData(contextId, &scaleX, &scaleY, &packetPropsCount, &packetProps);

		RealTimeStylusCursorContext context;

		for (ULONG axisIndex = 0; axisIndex < packetPropsCount; axisIndex++) {
			GUID axisGUID = packetProps[axisIndex].guid;
			PROPERTY_METRICS * propertyMetrics = &packetProps[axisIndex].PropertyMetrics;

			const StylusAxisContext axisContext(axisIndex, propertyMetrics->nLogicalMin, propertyMetrics->nLogicalMax);

			if (axisGUID == GUID_PACKETPROPERTY_GUID_NORMAL_PRESSURE) {
				context.axes[axisId(Axis::PRESSURE)] = axisContext;
			}
			else if (axisGUID == GUID_PACKETPROPERTY_GUID_TANGENT_PRESSURE) {
				context.axes[axisId(Axis::TANGENT_PRESSURE)] = axisContext;
			}
			else if (axisGUID == GUID_PACKETPROPERTY_GUID_X_TILT_ORIENTATION) {
				context.axes[axisId(Axis::TILT_X)] = axisContext;
			}
			else if (axisGUID == GUID_PACKETPROPERTY_GUID_Y_TILT_ORIENTATION) {
				context.axes[axisId(Axis::TILT_Y)] = axisContext;
			}
		}

		const StylusAxisContext & pressureContext = context.axes[axisId(Axis::PRESSURE)];

		if (pressureContext.axisId > 0) {
			std::ostringstream stringStream;
			stringStream << RealTimeStylusUtils::getName(inkCursor);
			stringStream << " (" << RealTimeStylusUtils::getName(inkTablet) << ")";

			context.name = stringStream.str();
			context.contextId = contextId;
			context.cursor = RealTimeStylusUtils::mapCursor(inkCursor);
			context.buttonMap = RealTimeStylusUtils::mapCursorButtons(inkCursor);

			contexts[sid] = context;
		}

		CoTaskMemFree(packetProps);
	}

	RealTimeStylusCursorContext * RealTimeStylusEventHandler::getContext(STYLUS_ID sid)
	{
		RealTimeStylusCursorContext * context = nullptr;

		try {
			context = &contexts.at(sid);
		}
		catch (const std::out_of_range & e) {
			// No context found.
			(void) e;
		}

		return context;
	}

	void RealTimeStylusEventHandler::createEvent(EventType type, const StylusInfo * pStylusInfo, ULONG & nPackets, ULONG & nPacketBuf, LONG * pPackets)
	{
		RealTimeStylusCursorContext * context = getContext(pStylusInfo->cid);

		if (context == nullptr) {
			return;
		}

		// Number of properties in a packet.
		ULONG nProps = nPacketBuf / nPackets;

		// Expect at least x and y coordinates.
		if (nProps < 2) {
			return;
		}

		for (ULONG i = 0; i < nPackets; i += nProps) {
			double x = pPackets[i];
			double y = pPackets[i + 1];

			// Since the packet data is in Ink Space coordinates, we need to convert to (sub)pixels.
			x *= dpiScaleX;
			y *= dpiScaleY;

			StylusAxesData axesData;
			axesData[Axis::X] = x;
			axesData[Axis::Y] = y;

			getAxesData(context, pPackets, i, nProps, axesData);

			auto event = std::make_shared<StylusEvent>(EventType::MOVE, button, context->cursor, std::move(axesData));

			eventDispatcher->dispatchEvent(event);
		}
	}

	void RealTimeStylusEventHandler::createMouseEvent(EventType type, WPARAM wParam, LPARAM lParam)
	{
		double x = GET_X_LPARAM(lParam);
		double y = GET_Y_LPARAM(lParam);

		Button button = Button::NONE;

		if (wParam & MK_LBUTTON) {
			button = Button::LEFT;
		}
		else if (wParam & MK_MBUTTON) {
			button = Button::MIDDLE;
		}
		else if (wParam & MK_RBUTTON) {
			button = Button::RIGHT;
		}

		StylusAxesData axesData;
		axesData[Axis::X] = x;
		axesData[Axis::Y] = y;
		axesData[Axis::PRESSURE] = 1;

		auto event = std::make_shared<StylusEvent>(type, button, Cursor::MOUSE, std::move(axesData));

		eventDispatcher->dispatchEvent(event);
	}

	void RealTimeStylusEventHandler::getAxesData(RealTimeStylusCursorContext * context, const LONG * packets, const ULONG & index, const ULONG & axesCount, StylusAxesData & axesData)
	{
		// Iterate over axes and assign axes values.
		for (Axis axis = Axis::PRESSURE; axis < Axis::COUNT; axis++) {
			const StylusAxisContext & axisContext = context->axes[axisId(axis)];

			const unsigned axisIndex = axisContext.axisId;

			// Retrieve axis data within packet bounds.
			if (axisIndex < axesCount) {
				axesData[axis] = packets[index + axisIndex] * (1.0f / axisContext.rangeValue);
			}
		}
	}

	LRESULT CALLBACK RealTimeStylusEventHandler::windowProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		LPMSG payload = reinterpret_cast<LPMSG>(lParam);
		RealTimeStylusEventHandler * context = windowMap[payload->hwnd];
		RealTimeStylusCursorContext * stylusContext = context->stylusContext;

		if (context == nullptr) {
			return 0;
		}

		if (nCode < 0 || context->stylusActive || stylusContext != nullptr) {
			// If the value of nCode is < 0, just pass it on.
			// This is required by the specification of WH_CALLWNDPROC handlers.
			return CallNextHookEx(context->hookHandle, nCode, wParam, lParam);
		}

		switch (payload->message) {
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
				context->createMouseEvent(EventType::BUTTON_DOWN, payload->wParam, payload->lParam);
				break;

			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
				context->createMouseEvent(EventType::BUTTON_UP, payload->wParam, payload->lParam);
				break;

			case WM_MOUSEMOVE:
				context->createMouseEvent(EventType::MOVE, payload->wParam, payload->lParam);
				break;

			default:
				break;
		}

		// Pass the message on to the next hook.
		return CallNextHookEx(context->hookHandle, nCode, wParam, lParam);
	}
}
