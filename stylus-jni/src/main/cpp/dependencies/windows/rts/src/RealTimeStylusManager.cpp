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

#include "RealTimeStylusManager.h"
#include "RealTimeStylusDevice.h"
#include "StylusException.h"
#include "StylusUtils.h"
#include "WinUtils.h"

#include <memory>
#include <initguid.h>
#include <guiddef.h>
#include <devguid.h>
#include <devpkey.h>
#include <propkey.h>
#include <setupapi.h>
#include <windows.h>

namespace stylus
{
	RealTimeStylusManager::RealTimeStylusManager() :
        StylusManager()
	{
		enumerateDevices();
	}

	RealTimeStylusManager::~RealTimeStylusManager()
	{
		windowHandlers.clear();
	}

	void RealTimeStylusManager::attachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener)
	{
		long windowId = GetJavaWindowId(jWindow.getEnv(), jWindow);

    	if (windowId == 0) {
    		throw StylusException("Window not found");
    	}

		HWND hWnd = reinterpret_cast<HWND>(LongToHandle(windowId));

		PRealTimeStylusEventHandler rtsHandler = nullptr;

		try {
			rtsHandler = windowHandlers.at(hWnd);
		}
		catch (const std::out_of_range & e) {
			// No handler found. Create a new one.
			(void) e;

			rtsHandler = std::make_shared<RealTimeStylusEventHandler>(hWnd, this);

			windowHandlers[hWnd] = rtsHandler;
		}

		StylusManager::attachStylusListener(listener);
	}

	void RealTimeStylusManager::detachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener)
	{
		long windowId = GetJavaWindowId(jWindow.getEnv(), jWindow);

    	if (windowId == 0) {
    		throw StylusException("Window not found");
    	}

		HWND hWnd = reinterpret_cast<HWND>(LongToHandle(windowId));

		size_t num = windowHandlers.erase(hWnd);

		if (num == 0) {
			throw StylusException("Detach StylusListener failed. Given window id '%ld' not bound.", windowId);
		}

		StylusManager::detachStylusListener(listener);
	}

	void RealTimeStylusManager::enumerateDevices()
	{
		HDEVINFO devList = SetupDiGetClassDevs(&GUID_DEVCLASS_MOUSE, nullptr, nullptr, DIGCF_PRESENT);

		if (!devList) {
			throw StylusException("Get device information set failed");
		}

		SP_DEVINFO_DATA devInfoData = { sizeof(SP_DEVINFO_DATA) };
		DWORD devIndex = 0, reqSize = 0;
		DEVPROPTYPE propType;

		while (SetupDiEnumDeviceInfo(devList, devIndex++, &devInfoData)) {
			wchar_t friendlyName[MAX_PATH] = { 0 };

			if (!SetupDiGetDevicePropertyW(devList, &devInfoData, &DEVPKEY_NAME, &propType, reinterpret_cast<PBYTE>(friendlyName), sizeof(friendlyName), &reqSize, 0)) {
				throw StylusException("Get device name failed");
			}

			std::string name = WideStrToStr(friendlyName);

			stylusDevices.insertDevice(std::make_shared<RealTimeStylusDevice>(name));
		}

		if (devList) {
			SetupDiDestroyDeviceInfoList(devList);
		}
	}
}
