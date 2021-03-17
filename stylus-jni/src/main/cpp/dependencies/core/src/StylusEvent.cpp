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

#include "StylusEvent.h"
#include "Convert.h"

#include <utility>

namespace stylus
{
	StylusEvent::StylusEvent(EventType type, Button button, Cursor cursor, StylusAxesData && axesData) :
		type(type),
		button(button),
		cursor(cursor),
		axesData(std::move(axesData))
	{
	}

	std::string StylusEvent::toString()
	{
		return stylus::toString(*this);
	}

	double StylusEvent::getX() const
	{
		return axesData[Axis::X];
	}

	double StylusEvent::getY() const
	{
		return axesData[Axis::Y];
	}

	double StylusEvent::getPressure() const
	{
		return axesData[Axis::PRESSURE];
	}

	double StylusEvent::getTangentPressure() const
	{
		return axesData[Axis::TANGENT_PRESSURE];
	}

	double StylusEvent::getTiltX() const
	{
		return axesData[Axis::TILT_X];
	}

	double StylusEvent::getTiltY() const
	{
		return axesData[Axis::TILT_Y];
	}

	std::ostream & operator <<(std::ostream & outputStream, const StylusEvent & event)
	{
		outputStream << "EventType: " << toInteger(event.type) << ", ";
		outputStream << "Cursor: " << toInteger(event.cursor) << ", ";
		outputStream << "Button: " << toInteger(event.button) << ", ";
		outputStream << "X: " << event.getX() << ", ";
		outputStream << "Y: " << event.getY() << ", ";
		outputStream << "Pressure: " << event.getPressure() << ", ";
		outputStream << "Tangent Pressure: " << event.getTangentPressure() << ", ";
		outputStream << "Tilt X: " << event.getX() << ", ";
		outputStream << "Tilt Y: " << event.getY();

		return outputStream;
	}
}
