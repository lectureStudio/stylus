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

#ifndef X11_STYLUS_UTILS_H_
#define X11_STYLUS_UTILS_H_

#include "Stylus.h"

#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

namespace stylus
{
	class X11Utils
	{
		public:
			static void checkXInputExtension(Display * display);
			static bool hasXI2(Display * display);
			static void getDefaultScreenSize(Display * display, int * width, int * height);
			static void getWindowLocation(Display * display, Window window, int * x, int * y);
			static void getWindowSize(Display * display, Window window, int * width, int * height);
			static void listDeviceProperties(Display * display, XDevice * device);
			static Button getButton(unsigned int buttonId);
	};
}

#endif
