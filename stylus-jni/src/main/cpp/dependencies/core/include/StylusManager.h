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

#ifndef STYLUS_MANAGER_H_
#define STYLUS_MANAGER_H_

#include "DeviceList.h"
#include "EventDispatcher.h"
#include "Stylus.h"
#include "StylusDevice.h"
#include "StylusEvent.h"
#include "StylusListener.h"
#include "JavaRef.h"
#include "Queue.h"

#include <atomic>
#include <list>
#include <memory>
#include <set>
#include <thread>

namespace stylus
{
	class StylusManager : public EventDispatcher<StylusEvent>
	{
		public:
			StylusManager();
			virtual ~StylusManager();

			StylusManager(const StylusManager &) = delete;
			StylusManager & operator=(const StylusManager &) = delete;

			virtual void attachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener);
			virtual void detachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener);

			void dispatchEvent(PStylusEvent event);

			std::set<PStylusDevice> getDevices();

		protected:
			void attachStylusListener(PStylusListener listener);
			void detachStylusListener(PStylusListener listener);

		private:
			void notifyListeners(PStylusEvent event);

			void dispatchLoop();
			void startCapture();
			void stopCapture();

		protected:
			DeviceList<PStylusDevice> stylusDevices;

		private:
			std::list<std::weak_ptr<StylusListener>> stylusListeners;

			std::thread thread;
			std::atomic<bool> captureEvents;

			Queue<PStylusEvent> eventQueue;
	};
}

#endif

