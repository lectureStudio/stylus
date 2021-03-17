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

#include "StylusAxisContext.h"

namespace stylus
{
	StylusAxisContext::StylusAxisContext() :
		axisId(0),
		minValue(0),
		maxValue(0),
		rangeValue(0)
	{
	}

	StylusAxisContext::StylusAxisContext(unsigned axisId, double min, double max) :
		axisId(axisId),
		minValue(min),
		maxValue(max),
		rangeValue(max - min)
	{
	}

	StylusAxisContext & StylusAxisContext::operator=(const StylusAxisContext & other)
	{
		if (this != &other) {
			axisId = other.axisId;
			minValue = other.minValue;
			maxValue = other.maxValue;
			rangeValue = other.rangeValue;

		}

		return *this;
	}
}
