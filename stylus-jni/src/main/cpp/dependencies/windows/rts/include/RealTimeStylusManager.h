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

#ifndef RTS_STYLUS_MANAGER_H_
#define RTS_STYLUS_MANAGER_H_

#include <unordered_map>
#include <Windows.h>

#include "RealTimeStylusEventHandler.h"
#include "StylusManager.h"
#include "windows/ComInitializer.h"

namespace stylus
{
	class RealTimeStylusManager : public StylusManager
	{
		public:
			RealTimeStylusManager();
			virtual ~RealTimeStylusManager();

			void attachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener) override;
			void detachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener) override;

		private:
			void enumerateDevices();

		private:
			jni::ComInitializer comInitializer;
			std::unordered_map<HWND, PRealTimeStylusEventHandler> windowHandlers;
	};
}

#endif
