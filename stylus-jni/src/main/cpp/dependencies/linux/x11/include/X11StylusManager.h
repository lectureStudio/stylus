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

#ifndef X11_STYLUS_MANAGER_H_
#define X11_STYLUS_MANAGER_H_

#include "StylusManager.h"
#include "X11InputDevice.h"

#include <atomic>
#include <thread>
#include <unordered_map>
#include <X11/Xlib.h>

namespace stylus
{
	class X11StylusManager : public StylusManager
	{
		public:
			X11StylusManager();
			virtual ~X11StylusManager();

			void attachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener) override;

		private:
			void initTabletDevices();
			PX11InputDevice createTabletDevice(XDeviceInfo * deviceInfo);
			void captureEvents();

		private:
			Display * display;

			std::unordered_map<XID, PX11InputDevice> devices;

			std::atomic<bool> running;
			std::thread thread;
	};
}

#endif
