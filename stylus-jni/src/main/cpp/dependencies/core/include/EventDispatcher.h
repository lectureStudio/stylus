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

#ifndef STYLUS_EVENT_DISPATCHER_H_
#define STYLUS_EVENT_DISPATCHER_H_

#include "Event.h"

#include <memory>
#include <type_traits>

namespace stylus
{
	template<class T>
	class EventDispatcher
	{
		static_assert(std::is_base_of<Event, T>::value, "T must derive from Event");

		public:
			virtual ~EventDispatcher() {};

			virtual void dispatchEvent(std::shared_ptr<T> event) = 0;
	};

	template<class T>
	using PEventDispatcher = std::shared_ptr<EventDispatcher<T>>;
}

#endif
