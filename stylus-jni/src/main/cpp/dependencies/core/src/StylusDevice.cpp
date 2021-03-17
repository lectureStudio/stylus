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

#include "StylusDevice.h"

namespace stylus
{
	StylusDevice::StylusDevice()
	{
	}

	bool StylusDevice::operator==(const StylusDevice & other)
	{
		return name == other.name;
	}
	
	bool StylusDevice::operator!=(const StylusDevice & other)
	{
		return !(*this == other);
	}
	
	bool StylusDevice::operator<(const StylusDevice & other)
	{
		return name < other.name;
	}

	std::string StylusDevice::getName() const
	{
		return name;
	}
}
