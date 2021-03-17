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

#include "CocoaStylusContext.h"
#include "StylusException.h"

namespace stylus
{
	CocoaStylusContext::CocoaStylusContext(EventDispatcher<StylusEvent> * eventDispatcher) :
		button(Button::NONE),
		cursor(Cursor::NONE),
		eventDispatcher(eventDispatcher)
	{
		if (eventDispatcher == nullptr) {
			throw StylusException("NullPointer: event dispatcher");
		}
	}

	void CocoaStylusContext::onButtonPressedEvent(NSEvent * event)
	{
		mapButton(event);

		dispatchStylusEvent(event, EventType::BUTTON_DOWN);
	}

	void CocoaStylusContext::onButtonReleasedEvent(NSEvent * event)
	{
		dispatchStylusEvent(event, EventType::BUTTON_UP);

		button = Button::NONE;

		NSUInteger pressedMouseButtons = [NSEvent pressedMouseButtons];

		if (pressedMouseButtons != 0) {
			if ((pressedMouseButtons & 1) != 0) {
				button = Button::LEFT;
			}
			else if ((pressedMouseButtons & 2) != 0) {
				button = Button::RIGHT;
			}
			else if ((pressedMouseButtons & 4) != 0) {
				button = Button::MIDDLE;
			}
		}

		EventType eventType = (button == Button::NONE) ? EventType::MOVE : EventType::BUTTON_DOWN;

		dispatchStylusEvent(event, eventType);
	}

	void CocoaStylusContext::onPointEvent(NSEvent * event)
	{
		dispatchStylusEvent(event, EventType::MOVE);
	}

	void CocoaStylusContext::onProximityEvent(NSEvent * event)
	{
		mapCursor(event);

		dispatchStylusEvent(event, EventType::CURSOR);
	}

	void CocoaStylusContext::dispatchStylusEvent(NSEvent * event, EventType eventType)
	{
		auto stylusEvent = std::make_shared<StylusEvent>(EventType::NONE, Button::NONE, Cursor::NONE, StylusAxesData());

		CGFloat scale = event.window.backingScaleFactor;
        NSPoint location = event.locationInWindow;

		StylusAxesData & axesData = stylusEvent->axesData;
        axesData[Axis::X] = location.x * scale;
        axesData[Axis::Y] = (event.window.contentLayoutRect.size.height - location.y) * scale;

		if (event.type == NSEventTypeTabletPoint || event.subtype == NSEventSubtypeTabletPoint) {
			// Tablet event.
			if (event.subtype == NSEventSubtypeTabletProximity) {
				mapCursor(event);
			}

			NSPoint tilt = event.tilt;

			axesData[Axis::PRESSURE] = event.pressure;
			axesData[Axis::TANGENT_PRESSURE] = event.tangentialPressure;
			axesData[Axis::TILT_X] = tilt.x;
			axesData[Axis::TILT_Y] = tilt.y;
			axesData[Axis::ROTATION] = event.rotation;
		}
		else {
			// Mouse event.
			mapCursor(event);

			axesData[Axis::PRESSURE] = (button == Button::NONE || eventType == EventType::BUTTON_UP) ? 0 : 1;
		}

		stylusEvent->type = eventType;
		stylusEvent->button = button;
		stylusEvent->cursor = cursor;

		eventDispatcher->dispatchEvent(stylusEvent);
	}

	void CocoaStylusContext::mapButton(NSEvent * event)
	{
		switch(event.buttonNumber) {
			case 0:
				button = Button::LEFT;
				break;

			case 1:
				button = Button::RIGHT;
				break;

			case 2:
				button = Button::MIDDLE;
				break;

			default:
				button = Button::NONE;
				break;
		}
	}

	void CocoaStylusContext::mapCursor(NSEvent * event)
	{
		switch (event.pointingDeviceType) {
			case NSPointingDeviceTypeEraser:
				cursor = Cursor::ERASER;
				break;

			case NSPointingDeviceTypePen:
			case NSPointingDeviceTypeCursor:
				cursor = Cursor::PEN;
				break;

			case NSPointingDeviceTypeUnknown:
			default:
				cursor = Cursor::MOUSE;
				break;
		}
	}
}
