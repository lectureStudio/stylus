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

#ifndef STYLUS_EVENT_H_
#define STYLUS_EVENT_H_

#include "StylusAxesData.h"
#include "Event.h"
#include "Stylus.h"

#include <string>
#include <memory>
#include <ostream>

namespace stylus
{
	/**
	 * Contains information about a tablet panel stylus event.
	 */
	class StylusEvent : public Event
	{
		public:
			StylusEvent(EventType type, Button button, Cursor cursor, StylusAxesData && axesData);
			virtual ~StylusEvent() {};

			/**
			 * The X coordinate of the stylus on the tablet device. The value is
			 * mapped to the screen space.
			 *
			 * @return The X coordinate.
			 */
			double getX() const;

			/**
			 * The Y coordinate of the stylus on the tablet device. The value is
			 * mapped to the screen space.
			 *
			 * @return The Y coordinate.
			 */
			double getY() const;

			/**
			 * The normalized pressure applied to the contact point in the range of
			 * [0,1], where 0 and 1 represent the minimum and maximum pressure.
			 *
			 * @return The normalized pressure.
			 */
			double getPressure() const;

			/**
			 * The normalized tangential pressure in the range of [0,1], where 0 and 1
			 * represent the minimum and maximum pressure.
			 *
			 * Note: Not all devices support tangential pressure. In this case the value
			 * is 0.
			 *
			 * @return The normalized tangential pressure.
			 */
			double getTangentPressure() const;

			/**
			 * The angle (in degrees, in the range of [-90,90]) between the Y-Z plane
			 * and the plane containing the stylus device axis and the Y axis. If the
			 * stylus is perfectly perpendicular to the tablet surface, the value is 0.
			 *
			 * If the device does not support tilt, the value is 0.
			 *
			 * @return The plane angle in degrees.
			 */
			double getTiltX() const;

			/**
			 * The angle (in degrees, in the range of [-90,90]) between the X-Z plane
			 * and the plane containing the stylus device axis and the X axis. If the
			 * stylus is perfectly perpendicular to the tablet surface, the value is 0.
			 *
			 * If the device does not support tilt, the value is 0.
			 *
			 * @return The plane angle in degrees.
			 */
			double getTiltY() const;

			/**
			 * The string representation of this event, that prints detailed information
			 * about the event type and axis values.
			 *
			 * @return The string representation of this event.
			 */
			std::string toString();

		private:
			friend std::ostream & operator <<(std::ostream & outputStream, const StylusEvent & event);

		public:
			/** Indicates the type of this event. */
			EventType type;

			/** Indicates the button type that caused the event. */
			Button button;

			/** Indicates the cursor type that caused the event. */
			Cursor cursor;

			/** The axis data. */
			StylusAxesData axesData;
	};


	using PStylusEvent = std::shared_ptr<StylusEvent>;
}

#endif
