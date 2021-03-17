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

#ifndef X11_STYLUS_MOUSE_DEVICE_H_
#define X11_STYLUS_MOUSE_DEVICE_H_

#include "X11InputDevice.h"

#include <memory>

namespace stylus
{
	class X11MouseDevice : public X11InputDevice
	{
		public:
			X11MouseDevice(Display * display, XDeviceInfo * deviceInfo, Cursor cursor);
			virtual ~X11MouseDevice();

			PStylusEvent getStylusEvent(XDeviceEvent * deviceEvent) override;

		private:
			void initEvents();
			PStylusEvent createMouseEvent(EventType type, int x, int y);
			PStylusEvent getButtonEvent(XDeviceEvent * deviceEvent);
			PStylusEvent getMotionEvent(XDeviceEvent * deviceEvent);
	};


	using PX11MouseDevice = std::shared_ptr<X11MouseDevice>;
}

#endif
