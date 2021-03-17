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

#ifndef COCOA_STYLUS_CONTEXT_H_
#define COCOA_STYLUS_CONTEXT_H_

#include "EventDispatcher.h"
#include "Stylus.h"
#include "StylusEvent.h"

#include <Cocoa/Cocoa.h>
#include <memory>

namespace stylus
{
	class CocoaStylusContext
	{
		public:
			CocoaStylusContext(EventDispatcher<StylusEvent> * eventDispatcher);
			virtual ~CocoaStylusContext() = default;

			void onButtonPressedEvent(NSEvent * event);
			void onButtonReleasedEvent(NSEvent * event);
			void onPointEvent(NSEvent * event);
			void onProximityEvent(NSEvent * event);

		private:
			void dispatchStylusEvent(NSEvent * event, EventType eventType);
			void translateToScreen(NSEvent * event, StylusAxesData & axesData);
			void mapButton(NSEvent * event);
			void mapCursor(NSEvent * event);

		private:
			Button button;
			Cursor cursor;

			EventDispatcher<StylusEvent> * eventDispatcher;
	};

	using PCocoaStylusContext = std::shared_ptr<CocoaStylusContext>;
}

#endif
