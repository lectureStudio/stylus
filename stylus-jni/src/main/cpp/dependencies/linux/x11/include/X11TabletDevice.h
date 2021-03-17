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

#ifndef X11_STYLUS_TABLET_DEVICE_H_
#define X11_STYLUS_TABLET_DEVICE_H_

#include "X11InputDevice.h"
#include "StylusAxisContext.h"

#include <memory>

namespace stylus
{
	class XAtoms
	{
		public:
			Atom axisLabels;
			Atom absX;
			Atom absY;
			Atom absPressure;
			Atom absTiltX;
			Atom absTiltY;

			static XAtoms & instance();

			XAtoms(const XAtoms &) = delete;
			void operator=(const XAtoms &) = delete;

		private:
			XAtoms();
	};


	class X11TabletDevice : public X11InputDevice
	{
		public:
			X11TabletDevice(Display * display, XDeviceInfo * deviceInfo, Cursor cursor);
			virtual ~X11TabletDevice() = default;

			void activate(Window window) override;

			PStylusEvent getStylusEvent(XDeviceEvent * deviceEvent) override;

		private:
			void initAxesMapping(XAnyClassPtr inputClassInfo);
			void initEvents();
			void initValuator(XValuatorInfoPtr valuator, Axis axis);
			void initValuators(XAnyClassPtr inputClassInfo);
			void setAxisContext(Atom axis, const StylusAxisContext & axisContext);
			void setAxisValue(StylusAxesData & axesData, const Axis & axis, const double & scalar, const int & modifier, const int * devAxes, const unsigned & axesCount);
			PStylusEvent createStylusEvent(EventType type, const int * devAxes, const unsigned & axesCount);
			PStylusEvent getMotionEvent(XDeviceEvent * deviceEvent);
			PStylusEvent getButtonEvent(XDeviceEvent * deviceEvent);
			PStylusEvent getProximityEvent(XDeviceEvent * deviceEvent);

		private:
			Window window;

			// Event types.
			int proximityInType;
			int proximityOutType;

			// Axis contexts.
			StylusAxisContext axes[7];

			// Axis atoms.
			XAtoms & axisAtoms;
	};


	using PX11TabletDevice = std::shared_ptr<X11TabletDevice>;
}

#endif
