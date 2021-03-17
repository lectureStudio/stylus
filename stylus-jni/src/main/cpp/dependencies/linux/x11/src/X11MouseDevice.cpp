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

#include "X11MouseDevice.h"
#include "X11Utils.h"

namespace stylus
{
	X11MouseDevice::X11MouseDevice(Display * display, XDeviceInfo * deviceInfo, Cursor cursor) :
		X11InputDevice(display, deviceInfo, cursor)
	{
		initEvents();
	}

	X11MouseDevice::~X11MouseDevice()
	{
	}

	PStylusEvent X11MouseDevice::getStylusEvent(XDeviceEvent * deviceEvent)
	{
		PStylusEvent stylusEvent = getButtonEvent(deviceEvent);

		if (stylusEvent) {
			return stylusEvent;
		}

		stylusEvent = getMotionEvent(deviceEvent);

		if (stylusEvent) {
			return stylusEvent;
		}

		return nullptr;
	}

	PStylusEvent X11MouseDevice::getMotionEvent(XDeviceEvent * deviceEvent)
	{
		if (deviceEvent->type != motionType) {
			return nullptr;
		}

		XDeviceMotionEvent * motionEvent = reinterpret_cast<XDeviceMotionEvent *>(deviceEvent);

		PStylusEvent stylusEvent = createMouseEvent(EventType::MOVE, motionEvent->x, motionEvent->y);

		return stylusEvent;
	}

	PStylusEvent X11MouseDevice::getButtonEvent(XDeviceEvent * deviceEvent)
	{
		bool pressed = deviceEvent->type == buttonPressType;
		bool released = deviceEvent->type == buttonReleaseType;

		if (!pressed && !released) {
			return nullptr;
		}

		XDeviceButtonEvent * buttonEvent = reinterpret_cast<XDeviceButtonEvent *>(deviceEvent);

		EventType type;

		if (pressed) {
			type = EventType::BUTTON_DOWN;

			button = X11Utils::getButton(buttonEvent->button);
		}
		else if (released) {
			type = EventType::BUTTON_UP;

			// Reset button state.
			button = Button::NONE;
		}
		else {
			// Invalid button action.
			return nullptr;
		}

		return createMouseEvent(type, buttonEvent->x, buttonEvent->y);
	}

	void X11MouseDevice::initEvents()
	{
		if (device->num_classes < 1) {
			return;
		}

		XInputClassInfo * ip;
		int i;

		for (ip = device->classes, i = 0; i < device->num_classes; ip++, i++) {
			switch (ip->input_class) {
				case ButtonClass:
					DeviceButtonPress(device, buttonPressType, eventList[eventCount]);

					if (eventList[eventCount]) {
						++eventCount;
					}

					DeviceButtonRelease(device, buttonReleaseType, eventList[eventCount]);

					if (eventList[eventCount]) {
						++eventCount;
					}
					break;

				case ValuatorClass:
					DeviceMotionNotify(device, motionType, eventList[eventCount]);

					if (eventList[eventCount]) {
						++eventCount;
					}
					break;

				default:
					break;
			}
		}
	}

	PStylusEvent X11MouseDevice::createMouseEvent(EventType type, int x, int y)
	{
		StylusAxesData axesData;
		axesData[Axis::X] = x;
		axesData[Axis::Y] = y;
		axesData[Axis::PRESSURE] = 1;

		return std::make_shared<StylusEvent>(type, button, cursor, std::move(axesData));
	}
}
