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

#ifndef X11_STYLUS_INPUT_DEVICE_H_
#define X11_STYLUS_INPUT_DEVICE_H_

#include "Stylus.h"
#include "StylusDevice.h"
#include "StylusEvent.h"

#include <memory>
#include <X11/extensions/XInput.h>

namespace stylus
{
	typedef struct
	{
		int type;				/* of event */
		unsigned long serial;	/* # of last request processed */
		Bool send_event;		/* true if from SendEvent request */
		Display * display;		/* Display the event was read from */
		Window window;			/* "event" window reported relative to */
		XID deviceid;
	} XDeviceEvent;


	class X11InputDevice : public StylusDevice
	{
		public:
			X11InputDevice(Display * display, XDeviceInfo * deviceInfo, Cursor cursor);
			virtual ~X11InputDevice();

			virtual void activate(Window window);
			Cursor getCursor();

			virtual PStylusEvent getStylusEvent(XDeviceEvent * deviceEvent) = 0;

		protected:
			Display * display;
			XDevice * device;
			XEventClass eventList[5];

			// Number of events registered.
			int eventCount;

			Button button;
			Cursor cursor;

			int screenWidth;
			int screenHeight;

			// Event types.
			int motionType;
			int buttonPressType;
			int buttonReleaseType;
	};


	using PX11InputDevice = std::shared_ptr<X11InputDevice>;
}

#endif
