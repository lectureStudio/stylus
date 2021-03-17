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

#ifndef RTS_STYLUS_EVENT_HANDLER_H_
#define RTS_STYLUS_EVENT_HANDLER_H_

#include "EventDispatcher.h"
#include "RealTimeStylusCursorContext.h"
#include "Stylus.h"
#include "StylusListener.h"

#include <ole2.h>
#include <rtscom.h>
#include <unordered_map>
#include <windows.h>

namespace stylus
{
	class RealTimeStylusEventHandler : public IStylusSyncPlugin
	{
		public:
			RealTimeStylusEventHandler(HWND hWnd, EventDispatcher<StylusEvent> * eventDispatcher);
			virtual ~RealTimeStylusEventHandler();

			STDMETHOD(DataInterest)(RealTimeStylusDataInterest * pEventInterest);
			STDMETHOD(Packets)(IRealTimeStylus * rtsSrc, const StylusInfo * pStylusInfo, ULONG nPackets, ULONG nPacketBuf, LONG * pPackets, ULONG * nOutPackets, LONG ** ppOutPackets);
			STDMETHOD(InAirPackets)(IRealTimeStylus * rtsSrc, const StylusInfo * pStylusInfo, ULONG, ULONG, LONG*, ULONG*, LONG**);
			STDMETHOD(StylusButtonDown)(IRealTimeStylus * rtsSrc, STYLUS_ID sid, const GUID * pGuidStylusButton, POINT * point);
			STDMETHOD(StylusButtonUp)(IRealTimeStylus * rtsSrc, STYLUS_ID sid, const GUID * pGuidStylusButton, POINT * point);
			STDMETHOD(StylusInRange)(IRealTimeStylus * rtsSrc, TABLET_CONTEXT_ID, STYLUS_ID sid);
			STDMETHOD(StylusOutOfRange)(IRealTimeStylus * rtsSrc, TABLET_CONTEXT_ID, STYLUS_ID sid);

			// Unused methods.
			STDMETHOD(TabletAdded)(IRealTimeStylus * rtsSrc, IInkTablet * inkTablet);
			STDMETHOD(TabletRemoved)(IRealTimeStylus * rtsSrc, LONG tabletIndex);
			STDMETHOD(StylusDown)(IRealTimeStylus*, const StylusInfo*, ULONG, LONG* _pPackets, LONG**);
			STDMETHOD(StylusUp)(IRealTimeStylus*, const StylusInfo*, ULONG, LONG* _pPackets, LONG**);
			STDMETHOD(RealTimeStylusEnabled)(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*);
			STDMETHOD(RealTimeStylusDisabled)(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*);
			STDMETHOD(SystemEvent)(IRealTimeStylus*, TABLET_CONTEXT_ID, STYLUS_ID, SYSTEM_EVENT, SYSTEM_EVENT_DATA);
			STDMETHOD(CustomStylusDataAdded)(IRealTimeStylus*, const GUID*, ULONG, const BYTE*);
			STDMETHOD(Error)(IRealTimeStylus*, IStylusPlugin*, RealTimeStylusDataInterest, HRESULT, LONG_PTR*);
			STDMETHOD(UpdateMapping)(IRealTimeStylus *);

			// IUnknown methods.
			STDMETHOD_(ULONG, AddRef)();
			STDMETHOD_(ULONG, Release)();

			STDMETHOD(QueryInterface)(REFIID riid, LPVOID * ppvObj);

		private:
			void initialize();
			void initializeWindowHook();
			void initializeCursors();
			void initializeCursor(IInkCursor * inkCursor);
			RealTimeStylusCursorContext * getContext(STYLUS_ID sid);
			void createEvent(EventType type, const StylusInfo * pStylusInfo, ULONG & nPackets, ULONG & nPacketBuf, LONG * pPackets);
			void createMouseEvent(EventType type, WPARAM wParam, LPARAM lParam);
			void getAxesData(RealTimeStylusCursorContext * context, const LONG * packets, const ULONG & index, const ULONG & axesCount, StylusAxesData & axesData);

			static LRESULT CALLBACK windowProc(int nCode, WPARAM wParam, LPARAM lParam);

		private:
			// Window handle to which the RTS is attached.
			HWND hWnd;

			// Window hook handle.
			HHOOK hookHandle;

			// StylusEvent dispatcher.
			EventDispatcher<StylusEvent> * eventDispatcher;

			// COM object reference count.
			LONG refCount;

			// Free-threaded marshaller.
			IUnknown * marshaller;

			// The stylus packet data handler.
			IRealTimeStylus * realTimeStylus;

			// Current pressed button.
			Button button;

			// Current stylus context in use.
			RealTimeStylusCursorContext * stylusContext;

			double dpiScaleX;
			double dpiScaleY;

			bool stylusActive;

			std::unordered_map<long, RealTimeStylusCursorContext> contexts;
	};


	using PRealTimeStylusEventHandler = std::shared_ptr<RealTimeStylusEventHandler>;
}

#endif
