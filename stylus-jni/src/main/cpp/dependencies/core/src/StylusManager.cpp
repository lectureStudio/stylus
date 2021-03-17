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

#include "StylusManager.h"

namespace stylus
{
	StylusManager::StylusManager() :
		thread(),
		captureEvents(false)
	{
		startCapture();
	}

	StylusManager::~StylusManager()
	{
		stylusDevices.clear();

		stopCapture();
	}

	void StylusManager::attachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener)
	{
		attachStylusListener(listener);
	}

	void StylusManager::detachStylusListener(const jni::JavaLocalRef<jobject> & jWindow, PStylusListener listener)
	{
		detachStylusListener(listener);
	}

	void StylusManager::dispatchEvent(PStylusEvent event)
	{
		eventQueue.push(event);
	}

	std::set<PStylusDevice> StylusManager::getDevices()
	{
		return stylusDevices.devices();
	}

	void StylusManager::attachStylusListener(PStylusListener listener)
	{
		stylusListeners.push_back(listener);
	}

	void StylusManager::detachStylusListener(PStylusListener listener)
	{
		stylusListeners.remove_if([listener](std::weak_ptr<StylusListener> p) {
			return !(p.owner_before(listener) || listener.owner_before(p));
		});
	}

	void StylusManager::notifyListeners(PStylusEvent event)
	{
		for (auto i = stylusListeners.begin(); i != stylusListeners.end();) {
			if ((*i).expired()) {
				i = stylusListeners.erase(i);
			}
			else {
				PStylusListener listener = (*i).lock();

				switch (event->type) {
					case EventType::CURSOR:
						listener->onCursorChange(event);
						break;

					case EventType::MOVE:
						listener->onCursorMove(event);
						break;

					case EventType::BUTTON_DOWN:
						listener->onButtonDown(event);
						break;

					case EventType::BUTTON_UP:
						listener->onButtonUp(event);
						break;

					default:
						break;
				}

				++i;
			}
		}
	}

	void StylusManager::dispatchLoop()
	{
		// Event dispatching loop.
		while (captureEvents) {
			auto event = eventQueue.pop();

			if (event == nullptr) {
				// Exit dispatching loop.
				break;
			}

			notifyListeners(event);
		}
	}

	void StylusManager::startCapture()
	{
		captureEvents = true;

		thread = std::thread(&StylusManager::dispatchLoop, this);
	}

	void StylusManager::stopCapture()
	{
		captureEvents = false;
		dispatchEvent(nullptr);
		thread.join();
	}
}
