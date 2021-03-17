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

#ifndef RTS_STYLUS_UTILS_H_
#define RTS_STYLUS_UTILS_H_

#include <msinkaut.h>
#include <string>
#include <unordered_map>
#include <Windows.h>

#include "Stylus.h"
#include "WinUtils.h"

namespace stylus
{
	class RealTimeStylusUtils
	{
		public:
			static std::string getName(IInkCursor * inkCursor);
			static std::string getName(IInkTablet * inkTablet);

			static Button mapButton(IInkCursorButton * cursorButton);
			static Cursor mapCursor(IInkCursor * inkCursor);

			static std::unordered_map<GUID, Button, GUIDHasher> mapCursorButtons(IInkCursor * inkCursor);

		private:
			static constexpr LPCWSTR BUTTON_LEFT_STR_ID  = L"Tip Switch";
			static constexpr LPCWSTR BUTTON_RIGHT_STR_ID = L"Barrel Switch";

			static constexpr LPCWSTR MOUSE_STR_ID  = L"Mouse";
			static constexpr LPCWSTR STYLUS_STR_ID = L"Stylus";
			static constexpr LPCWSTR ERASER_STR_ID = L"Eraser";
	};
}

#endif
