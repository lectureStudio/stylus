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

#include "X11Utils.h"
#include "StylusException.h"

namespace stylus
{
	void X11Utils::checkXInputExtension(Display * display)
	{
		int opcode, event, error;

		if (!XQueryExtension(display, "XInputExtension", &opcode, &event, &error)) {
			throw StylusException("X input extension is not available");
		}
	}

	bool X11Utils::hasXI2(Display * display)
	{
		int major = 2;
		int minor = 0;

		int rc = XIQueryVersion(display, &major, &minor);

		if (rc != Success) {
			return false;
		}

		return true;
	}

	void X11Utils::getDefaultScreenSize(Display * display, int * width, int * height)
	{
		Screen * screen = DefaultScreenOfDisplay(display);

		if (screen == nullptr) {
			throw StylusException("Obtain the default screen failed");
		}

		*width = screen->width;
		*height = screen->height;
	}

	void X11Utils::getWindowLocation(Display * display, Window window, int * x, int * y)
	{
		XWindowAttributes attributes;

		if (!XGetWindowAttributes(display, window, &attributes)) {
			throw StylusException("Get window attributes failed");
		}

		Window child;
		XTranslateCoordinates(display, window, attributes.root, 0, 0, x, y, &child);
	}

	void X11Utils::getWindowSize(Display * display, Window window, int * width, int * height)
	{
		XWindowAttributes attributes;

		if (!XGetWindowAttributes(display, window, &attributes)) {
			throw StylusException("Get window attributes failed");
		}

		*width = attributes.width;
		*height = attributes.height;
	}

	void X11Utils::listDeviceProperties(Display * display, XDevice * device)
	{
		int atomCount;

		Atom * atoms = XListDeviceProperties(display, device, &atomCount);

		if (atomCount > 0) {
			for (int j = 0; j < atomCount; j++) {
				char * name = XGetAtomName(display, atoms[j]);
				printf("Property: %s\n", name);
				XFree(name);
			}
		}

		XFree(atoms);
	}

	Button X11Utils::getButton(unsigned int buttonId)
	{
		switch (buttonId) {
			case 1:
				return Button::LEFT;
			case 2:
				return Button::MIDDLE;
			case 3:
				return Button::RIGHT;
			default:
				return Button::NONE;
		}
	}
}
