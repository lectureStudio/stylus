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

#include "X11InputDevice.h"
#include "StylusException.h"
#include "X11Utils.h"

namespace stylus
{
	X11InputDevice::X11InputDevice(Display * display, XDeviceInfo * deviceInfo, Cursor cursor) :
		StylusDevice(),
		display(display),
		device(nullptr),
		eventCount(0),
		button(Button::NONE),
		cursor(cursor),
		screenWidth(0),
		screenHeight(0),
		motionType(-1),
		buttonPressType(-1),
		buttonReleaseType(-1)
	{
		if (display == nullptr) {
			throw StylusException("Not connected to the X Server");
		}
		if (deviceInfo == nullptr) {
			throw StylusException("No device information provided");
		}

		device = XOpenDevice(display, deviceInfo->id);

		if (device == nullptr) {
			throw StylusException("Open device %s failed", deviceInfo->name ? deviceInfo->name : "<noname>");
		}

		if (deviceInfo->name != nullptr) {
			name = std::string(deviceInfo->name);
		}
	}

	X11InputDevice::~X11InputDevice()
	{
		XCloseDevice(display, device);
	}

	void X11InputDevice::activate(Window window)
	{
		if (XSelectExtensionEvent(display, window, eventList, eventCount)) {
			throw StylusException("Select device events failed");
		}

		X11Utils::getWindowSize(display, DefaultRootWindow(display), &screenWidth, &screenHeight);
	}

	Cursor X11InputDevice::getCursor()
	{
		return cursor;
	}
}
