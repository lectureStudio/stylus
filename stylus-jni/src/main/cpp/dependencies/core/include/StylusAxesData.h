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

#ifndef STYLUS_AXES_DATA_H_
#define STYLUS_AXES_DATA_H_

#include "Stylus.h"

#include <cstddef>
#include <array>

namespace stylus
{
	class StylusAxesData
	{
		public:
			StylusAxesData();
			StylusAxesData(StylusAxesData && other);
			virtual ~StylusAxesData() {};

			double & operator[](Axis axis);
			const double & operator[](Axis axis) const;

			StylusAxesData & operator=(StylusAxesData && other);

			size_t size();
			size_t size() const;

			const double * getData();
			const double * getData() const;

		private:
			std::array<double, static_cast<unsigned>(Axis::COUNT)> data;
	};
}

#endif
