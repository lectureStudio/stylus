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

#include "X11StylusManager.h"
#include "X11MouseDevice.h"
#include "X11TabletDevice.h"
#include "X11Utils.h"
#include "Stylus.h"
#include "StylusException.h"
#include "StylusUtils.h"

#include <algorithm>
#include <string>
#include <map>
#include <X11/Xatom.h>
#include <X11/extensions/XInput.h>

namespace stylus
{
	enum class XAtom
	{
		// Wacom old, before version 0.10
		WacomStylus,
		WacomCursor,
		WacomEraser,

		// (Wacom) device
		TabletStylus,
		TabletEraser,
		TabletTouch,

		// XInput Tablet
		InputTablet,

		// Aiptek device
		AiptekStylus,

		// UGTABLET TABLET G5 9x6
		WizardPenTablet,

		Mouse,
		TouchPad,

		XWayland,
		XWaylandStylus,
		XWaylandEraser,
		XWaylandPuck
	};

	static std::map<XAtom, std::string> atomKeyMap = {
		{ XAtom::WacomStylus,		"Wacom Stylus" },
		{ XAtom::WacomCursor,		"Wacom Cursor" },
		{ XAtom::WacomEraser,		"Wacom Eraser" },
		{ XAtom::Mouse,				"MOUSE" },
		{ XAtom::TouchPad,			"TOUCHPAD" },
		{ XAtom::TabletStylus,		"STYLUS" },
		{ XAtom::TabletEraser,		"ERASER" },
		{ XAtom::TabletTouch,		"TOUCH" },
		{ XAtom::InputTablet,		"TABLET" },
		{ XAtom::AiptekStylus,		"Stylus" },
		{ XAtom::WizardPenTablet,	"WizardPen Tablet" },
		{ XAtom::XWayland,			"xwayland-pointer" },
		{ XAtom::XWaylandStylus,	"xwayland-tablet stylus" },
		{ XAtom::XWaylandEraser,	"xwayland-tablet eraser" },
		{ XAtom::XWaylandPuck,		"xwayland-tablet cursor" }
	};

	static std::map<Atom, XAtom> atomMap;


	static void initInternAtoms(Display * display)
	{
		for (auto const & entry : atomKeyMap) {
			Atom atom = XInternAtom(display, entry.second.c_str(), False);
			atomMap[atom] = entry.first;
		}
	}


	X11StylusManager::X11StylusManager() :
		StylusManager(),
		display(nullptr),
		running(false),
		thread()
	{
		display = XOpenDisplay(NULL);

		if (display == nullptr) {
			throw StylusException("Unable to connect to the X Server");
		}

		X11Utils::checkXInputExtension(display);

		initInternAtoms(display);
		initTabletDevices();
	}

	X11StylusManager::~X11StylusManager()
	{
		// Shutdown gracefully.
		if (running) {
			running = false;

			Window window = DefaultRootWindow(display);

			Display * dsp = XOpenDisplay(NULL);

			// Send property-change-event to unblock the event-capture thread.
			unsigned char data[0];
			XChangeProperty(dsp, window, XA_ATOM, XA_ATOM, 8, PropModeAppend, data, 0);
			XFlush(dsp);

			// Close the display now that the event is sent.
			XCloseDisplay(dsp);

			if (thread.joinable()) {
				thread.join();
			}
		}

		// Release devices.
		devices.clear();

        // Causes segmentation fault.
//		if (display != nullptr) {
//			XCloseDisplay(display);
//			display = nullptr;
//		}
	}

	void X11StylusManager::attachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener)
	{
		long windowId = GetJavaWindowId(jWindow.getEnv(), jWindow);

		if (windowId == 0) {
			throw StylusException("Window not found");
		}

		Window window = static_cast<Window>(windowId);

		for (const PStylusDevice & device : getDevices()) {
			PX11InputDevice x11Device = std::static_pointer_cast<X11InputDevice>(device);
			x11Device->activate(window);
		}

		StylusManager::attachStylusListener(listener);

		if (!running) {
			running = true;

			thread = std::thread(&X11StylusManager::captureEvents, this);
			thread.detach();
		}
	}

	void X11StylusManager::initTabletDevices()
	{
		Window window = DefaultRootWindow(display);

		if (0 > window) {
			throw StylusException("Obtain the default root window failed");
		}

		int deviceCount;
		XDeviceInfo * devs = XListInputDevices(display, &deviceCount);
		XDeviceInfo * dev;

		for (int i = 0; i < deviceCount; i++) {
			dev = &devs[i];

			PX11InputDevice device = createTabletDevice(dev);

			if (device != nullptr) {
				devices[dev->id] = device;

				stylusDevices.insertDevice(device);
			}
		}

		XFreeDeviceList(devs);
	}

	PX11InputDevice X11StylusManager::createTabletDevice(XDeviceInfo * deviceInfo)
	{
		if (deviceInfo->use != IsXExtensionPointer) {
			return nullptr;
		}

		std::map<Atom, XAtom>::iterator it = atomMap.find(deviceInfo->type);

		if (it == atomMap.end()) {
			return nullptr;
		}

		PX11InputDevice device = nullptr;
		XAtom xAtom = it->second;

		if (xAtom == XAtom::Mouse || xAtom == XAtom::TouchPad || xAtom == XAtom::XWayland) {
			device = std::make_shared<X11MouseDevice>(display, deviceInfo, Cursor::MOUSE);
		}
		else if (xAtom == XAtom::TabletEraser || xAtom == XAtom::WacomEraser || xAtom == XAtom::XWaylandEraser) {
			device = std::make_shared<X11TabletDevice>(display, deviceInfo, Cursor::ERASER);
		}
		else {
			device = std::make_shared<X11TabletDevice>(display, deviceInfo, Cursor::PEN);
		}

		return device;
	}

	void X11StylusManager::captureEvents()
	{
		XEvent event;
		XDeviceEvent * deviceEvent;
		std::unordered_map<XID, PX11InputDevice>::iterator it;

		while (running) {
			XNextEvent(display, &event);

			deviceEvent = reinterpret_cast<XDeviceEvent *>(&event);

			// Get the device that fired the event.
			it = devices.find(deviceEvent->deviceid);

			if (it == devices.end()) {
				continue;
			}

			PX11InputDevice device = it->second;

			PStylusEvent stylusEvent = device->getStylusEvent(deviceEvent);

			if (stylusEvent) {
				dispatchEvent(stylusEvent);
			}
		}
	}
}
