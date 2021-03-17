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

#include "StylusException.h"

namespace stylus
{
	const unsigned int MAX_MESSAGE_LEN = 1024;

	StylusException::StylusException() : std::exception()
	{
	}

	StylusException::StylusException(const char * msg, ...) : std::exception()
	{
		char buffer[MAX_MESSAGE_LEN];

		va_list args;
		va_start(args, msg);
		vsnprintf(buffer, MAX_MESSAGE_LEN, msg, args);
		va_end(args);

		message = buffer;
	}

	const char * StylusException::what() const NOEXCEPT
	{
		return message.c_str();
	}
}