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

#include "RealTimeStylusUtils.h"

namespace stylus
{
	std::string RealTimeStylusUtils::getName(IInkCursor * inkCursor)
	{
		BSTR name;
		std::string str;
		HRESULT hr = inkCursor->get_Name(&name);

		if (SUCCEEDED(hr)) {
			str = BSTRToStr(name);

			SysFreeString(name);
		}

		return str;
	}

	std::string RealTimeStylusUtils::getName(IInkTablet * inkTablet)
	{
		BSTR name;
		std::string str;
		HRESULT hr = inkTablet->get_Name(&name);

		if (SUCCEEDED(hr)) {
			str = BSTRToStr(name);

			SysFreeString(name);
		}

		return str;
	}

	Button RealTimeStylusUtils::mapButton(IInkCursorButton * cursorButton)
	{
		Button button = Button::NONE;
		BSTR name;
		HRESULT hr = cursorButton->get_Name(&name);

		if (SUCCEEDED(hr)) {
			if (lstrcmpW(name, BUTTON_LEFT_STR_ID) == 0) {
				button = Button::LEFT;
			}
			else if (lstrcmpW(name, BUTTON_RIGHT_STR_ID) == 0) {
				button = Button::RIGHT;
			}

			SysFreeString(name);
		}

		return button;
	}

	Cursor RealTimeStylusUtils::mapCursor(IInkCursor * inkCursor)
	{
		Cursor cursor = Cursor::NONE;
		BSTR name;
		HRESULT hr = inkCursor->get_Name(&name);

		if (SUCCEEDED(hr)) {
			if (lstrcmpW(name, MOUSE_STR_ID) == 0) {
				cursor = Cursor::MOUSE;
			}
			else if (lstrcmpW(name, STYLUS_STR_ID) == 0) {
				cursor = Cursor::PEN;
			}
			else if (lstrcmpW(name, ERASER_STR_ID) == 0) {
				cursor = Cursor::ERASER;
			}

			SysFreeString(name);
		}

		return cursor;
	}

	std::unordered_map<GUID, Button, GUIDHasher> RealTimeStylusUtils::mapCursorButtons(IInkCursor * inkCursor)
	{
		std::unordered_map<GUID, Button, GUIDHasher> map;

		IInkCursorButtons * cursorButtons;

		HRESULT hr = inkCursor->get_Buttons(&cursorButtons);
		if (FAILED(hr)) {
			return map;
		}

		LONG buttonCount;

		cursorButtons->get_Count(&buttonCount);
		if (FAILED(hr)) {
			return map;
		}

		IInkCursorButton * cursorButton;
		VARIANT index;
		VariantInit(&index);
		index.vt = VT_I4;

		for (index.intVal = 0; index.intVal < buttonCount; index.intVal++) {
			hr = cursorButtons->Item(index, &cursorButton);
			if (FAILED(hr)) {
				continue;
			}

			BSTR id;

			hr = cursorButton->get_Id(&id);
			if (FAILED(hr)) {
				continue;
			}

			GUID buttonGUID;

			hr = IIDFromString(id, &buttonGUID);
			if (FAILED(hr)) {
				fwprintf(stderr, L"Get button GUID from string %s failed\n", id);
			}
			else {
				map[buttonGUID] = mapButton(cursorButton);
			}

			SysFreeString(id);
		}

		VariantClear(&index);

		return map;
	}
}
