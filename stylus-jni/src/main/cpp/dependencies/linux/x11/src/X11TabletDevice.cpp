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

#include "X11TabletDevice.h"
#include "X11Utils.h"

#include <math.h>

namespace stylus
{
	XAtoms & XAtoms::instance()
	{
		static XAtoms instance;
		return instance;
	}

	XAtoms::XAtoms()
	{
		Display * display = XOpenDisplay(NULL);

		axisLabels = XInternAtom(display, "Axis Labels", False);
		absX = XInternAtom(display, "Abs X", False);
		absY = XInternAtom(display, "Abs Y", False);
		absPressure = XInternAtom(display, "Abs Pressure", False);
		absTiltX = XInternAtom(display, "Abs Tilt X", False);
		absTiltY = XInternAtom(display, "Abs Tilt Y", False);

		XCloseDisplay(display);
	}

	X11TabletDevice::X11TabletDevice(Display * display, XDeviceInfo * deviceInfo, Cursor cursor) :
		X11InputDevice(display, deviceInfo, cursor),
		window(0),
		proximityInType(-1),
		proximityOutType(-1),
		axisAtoms(XAtoms::instance())
	{
		initEvents();
		initAxesMapping(deviceInfo->inputclassinfo);
	}

	void X11TabletDevice::activate(Window window)
	{
		this->window = window;

		X11InputDevice::activate(window);
	}

	PStylusEvent X11TabletDevice::getStylusEvent(XDeviceEvent * deviceEvent)
	{
		PStylusEvent stylusEvent = getButtonEvent(deviceEvent);

		if (stylusEvent) {
			return stylusEvent;
		}

		stylusEvent = getMotionEvent(deviceEvent);

		if (stylusEvent) {
			return stylusEvent;
		}

		stylusEvent = getProximityEvent(deviceEvent);

		if (stylusEvent) {
			return stylusEvent;
		}

		return nullptr;
	}

	PStylusEvent X11TabletDevice::getMotionEvent(XDeviceEvent * deviceEvent)
	{
		if (deviceEvent->type != motionType) {
			return nullptr;
		}

		XDeviceMotionEvent * motionEvent = reinterpret_cast<XDeviceMotionEvent *>(deviceEvent);

		EventType type = EventType::MOVE;
		unsigned axesCount = motionEvent->axes_count;
		PStylusEvent stylusEvent = createStylusEvent(type, motionEvent->axis_data, axesCount);
		StylusAxesData & axesData = stylusEvent->axesData;

		/*
		 * Observed on Linux with Wayland.
		 */
		if (button != Button::NONE && isinf(axesData[Axis::PRESSURE])) {
			axesData[Axis::PRESSURE] = 1;
		}

		/*
		 * For some reason the motion event with pressure > 0 is fired
		 * before the button-press event.
		 */
		if (axesData[Axis::PRESSURE] > 0 && button == Button::NONE) {
			return nullptr;
		}
		else if (axesData[Axis::PRESSURE] == 0 && button != Button::NONE) {
			return nullptr;
		}

		return stylusEvent;
	}

	PStylusEvent X11TabletDevice::getButtonEvent(XDeviceEvent * deviceEvent)
	{
		bool pressed = deviceEvent->type == buttonPressType;
		bool released = deviceEvent->type == buttonReleaseType;

		if (!pressed && !released) {
			return nullptr;
		}

		XDeviceButtonEvent * buttonEvent = reinterpret_cast<XDeviceButtonEvent *>(deviceEvent);

		button = X11Utils::getButton(buttonEvent->button);

		EventType type;

		if (pressed) {
			type = EventType::BUTTON_DOWN;
		}
		else if (released) {
			type = EventType::BUTTON_UP;
		}
		else {
			// Invalid button action.
			return nullptr;
		}

		unsigned axesCount = buttonEvent->axes_count;
		PStylusEvent stylusEvent = createStylusEvent(type, buttonEvent->axis_data, axesCount);

		if (released) {
			// Reset button state.
			button = Button::NONE;
		}

		if (axesCount == 0) {
			// Compatibility mode.
			int windowX = 0;
			int windowY = 0;

			X11Utils::getWindowLocation(display, window, &windowX, &windowY);

			StylusAxesData & axesData = stylusEvent->axesData;
			axesData[Axis::X] = buttonEvent->x;
			axesData[Axis::Y] = buttonEvent->y;
			axesData[Axis::PRESSURE] = 1;
		}

		return stylusEvent;
	}

	PStylusEvent X11TabletDevice::getProximityEvent(XDeviceEvent * deviceEvent)
	{
		bool proxIn = deviceEvent->type == proximityInType;
		bool proxOut = deviceEvent->type == proximityOutType;

		if (!proxIn && !proxOut) {
			return nullptr;
		}

		XProximityNotifyEvent * proximityEvent = reinterpret_cast<XProximityNotifyEvent *>(deviceEvent);

		unsigned axesCount = proximityEvent->axes_count;
		EventType type = EventType::CURSOR;

		return createStylusEvent(type, proximityEvent->axis_data, axesCount);
	}

	void X11TabletDevice::initEvents()
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

					ProximityIn(device, proximityInType, eventList[eventCount]);

					if (eventList[eventCount]) {
						++eventCount;
					}

					ProximityOut(device, proximityOutType, eventList[eventCount]);

					if (eventList[eventCount]) {
						++eventCount;
					}
					break;

				default:
					break;
			}
		}
	}

	void X11TabletDevice::initAxesMapping(XAnyClassPtr inputClassInfo)
	{
		Atom propertyType;
		int propertyFormat;
		unsigned long axesCount;
		unsigned long bytesAfter;
		unsigned char * prop;

		int result = XGetDeviceProperty(display, device, axisAtoms.axisLabels, 0, 16, false, AnyPropertyType,
				&propertyType, &propertyFormat, &axesCount, &bytesAfter, &prop);

		if (result == Success && propertyType > 0) {
			Atom * axesAtoms = reinterpret_cast<Atom *>(prop);

			for (unsigned int axisIndex = 0; axisIndex < axesCount; axisIndex++) {
				const StylusAxisContext axisContext(axisIndex, 0, 0);
				const Atom axis = axesAtoms[axisIndex];

				setAxisContext(axis, axisContext);
			}

			XFree(prop);

			initValuators(inputClassInfo);
		}
		else if (X11Utils::hasXI2(display)) {
			int deviceCount;

			XIDeviceInfo * info = XIQueryDevice(display, device->device_id, &deviceCount);

			if (deviceCount == 1) {
				XIDeviceInfo * dev = &info[0];

				for (int i = 0; i < dev->num_classes; i++) {
					switch (dev->classes[i]->type) {
						case XIValuatorClass:
						{
							XIValuatorClassInfo * v = reinterpret_cast<XIValuatorClassInfo *>(dev->classes[i]);
							Atom axis = v->label;
							StylusAxisContext axisContext(v->number, v->min, v->max);

							setAxisContext(axis, axisContext);
						}
							break;
					}
				}
			}
		}
		else {
			// Couldn't find axis mapping. Set default indices.

			axes[axisId(Axis::X)] = StylusAxisContext(0, 0, 0);
			axes[axisId(Axis::Y)] = StylusAxisContext(1, 0, 0);
			axes[axisId(Axis::PRESSURE)] = StylusAxisContext(2, 0, 0);
			axes[axisId(Axis::TILT_X)] = StylusAxisContext(3, 0, 0);
			axes[axisId(Axis::TILT_Y)] = StylusAxisContext(4, 0, 0);

			initValuators(inputClassInfo);
		}
	}

	void X11TabletDevice::initValuator(XValuatorInfoPtr valuator, Axis axis)
	{
		StylusAxisContext * axisContext = &axes[axisId(axis)];

		int min = valuator->axes[axisContext->axisId].min_value;
		int max = valuator->axes[axisContext->axisId].max_value;

		axisContext->minValue = min;
		axisContext->maxValue = max;
		axisContext->rangeValue = max - min;
	}

	void X11TabletDevice::initValuators(XAnyClassPtr inputClassInfo) {
		if (device->num_classes < 1) {
			return;
		}

		XAnyClassPtr any = inputClassInfo;

		for (int i = 0; i < device->num_classes; i++) {
			if (any->c_class == ValuatorClass) {
				XValuatorInfoPtr v = reinterpret_cast<XValuatorInfoPtr>(any);

				initValuator(v, Axis::X);
				initValuator(v, Axis::Y);
				initValuator(v, Axis::PRESSURE);
				initValuator(v, Axis::TILT_X);
				initValuator(v, Axis::TILT_Y);

				break;
			}

			any = reinterpret_cast<XAnyClassPtr>((char *) any + any->length);
		}
	}

	void X11TabletDevice::setAxisContext(Atom axis, const StylusAxisContext & axisContext)
	{
		if (axis == axisAtoms.absX) {
			axes[axisId(Axis::X)] = axisContext;
		}
		else if (axis == axisAtoms.absY) {
			axes[axisId(Axis::Y)] = axisContext;
		}
		else if (axis == axisAtoms.absPressure) {
			axes[axisId(Axis::PRESSURE)] = axisContext;
		}
		else if (axis == axisAtoms.absTiltX) {
			axes[axisId(Axis::TILT_X)] = axisContext;
		}
		else if (axis == axisAtoms.absTiltY) {
			axes[axisId(Axis::TILT_Y)] = axisContext;
		}
	}

	void X11TabletDevice::setAxisValue(StylusAxesData & axesData, const Axis & axis, const double & scalar, const int & modifier, const int * devAxes, const unsigned & axesCount)
	{
		const StylusAxisContext * context = &axes[axisId(axis)];

		if (context->axisId < axesCount) {
			axesData[axis] = (devAxes[context->axisId] / context->rangeValue) * scalar - modifier;
		}
	}

	PStylusEvent X11TabletDevice::createStylusEvent(EventType type, const int * devAxes, const unsigned & axesCount)
	{
		StylusAxesData axesData;

		if (axesCount > 0) {
			int windowX = 0;
			int windowY = 0;

			X11Utils::getWindowLocation(display, window, &windowX, &windowY);

			setAxisValue(axesData, Axis::X, screenWidth, windowX, devAxes, axesCount);
			setAxisValue(axesData, Axis::Y, screenHeight, windowY, devAxes, axesCount);
			setAxisValue(axesData, Axis::PRESSURE, 1, 0, devAxes, axesCount);
			setAxisValue(axesData, Axis::TILT_X, 1, 0, devAxes, axesCount);
			setAxisValue(axesData, Axis::TILT_Y, 1, 0, devAxes, axesCount);
		}

		return std::make_shared<StylusEvent>(type, button, cursor, std::move(axesData));
	}
}
