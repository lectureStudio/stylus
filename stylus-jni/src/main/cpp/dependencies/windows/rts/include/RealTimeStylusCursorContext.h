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

#ifndef RTS_STYLUS_CURSOR_CONTEXT_H_
#define RTS_STYLUS_CURSOR_CONTEXT_H_

#include "Stylus.h"
#include "StylusAxisContext.h"
#include "WinUtils.h"

#include <ole2.h>
#include <rtscom.h>
#include <string>
#include <unordered_map>
#include <windows.h>

namespace stylus
{
	class RealTimeStylusCursorContext
	{
		public:
			RealTimeStylusCursorContext();
			virtual ~RealTimeStylusCursorContext() = default;

		public:
			Button getButton(const GUID * guid);

		public:
			std::string name;
			TABLET_CONTEXT_ID contextId;
			Cursor cursor;

			StylusAxisContext axes[static_cast<unsigned>(Axis::COUNT)];

			std::unordered_map<GUID, Button, GUIDHasher> buttonMap;
	};
}

#endif
