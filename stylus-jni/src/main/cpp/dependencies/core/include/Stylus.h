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

#ifndef STYLUS_H_
#define STYLUS_H_

namespace stylus
{
	enum class Axis : unsigned
	{
		X,
		Y,
		PRESSURE,
		TANGENT_PRESSURE,
		TILT_X,
		TILT_Y,
		ROTATION,
		COUNT
	};

	enum class Button : unsigned
	{
		NONE,
		LEFT,
		MIDDLE,
		RIGHT
	};

	enum class Cursor : unsigned
	{
		NONE,
		MOUSE,
		ERASER,
		PEN
	};

	enum class EventType : unsigned
	{
		NONE,
		MOVE,
		BUTTON_DOWN,
		BUTTON_UP,
		CURSOR
	};


	inline unsigned axisId(const Axis & axis)
	{
		return static_cast<unsigned>(axis);
	}

	inline Axis & operator++(Axis & axis, int)
	{
		return axis = (axis == Axis::COUNT) ? Axis::X : static_cast<Axis>(axisId(axis) + 1);
	}
}

#endif
