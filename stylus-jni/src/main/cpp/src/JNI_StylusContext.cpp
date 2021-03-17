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

#include "JNI_StylusContext.h"
#include "JNI_Stylus.h"
#include "JavaEnums.h"
#include "JavaUtils.h"

#ifdef _WIN32
#include "RealTimeStylusManager.h"
#endif
#ifdef __linux__
#include "X11StylusManager.h"
#endif
#ifdef __APPLE__
#include "CocoaStylusManager.h"
#endif

namespace stylus
{
	JNI_StylusContext::JNI_StylusContext(JavaVM * vm) :
		jni::JavaContext(vm)
	{
	}

	StylusManager * JNI_StylusContext::getStylusManager()
	{
		return manager.get();
	}

	void JNI_StylusContext::initialize(JNIEnv * env)
	{
		jni::JavaContext::initialize(env);

		jni::JavaEnums::add<Button>(env, PKG "StylusButton");
		jni::JavaEnums::add<Cursor>(env, PKG "StylusCursor");

#ifdef _WIN32
		manager = std::make_unique<RealTimeStylusManager>();
#endif
#ifdef __linux__
		manager = std::make_unique<X11StylusManager>();
#endif
#ifdef __APPLE__
		manager = std::make_unique<CocoaStylusManager>();
#endif
	}

	void JNI_StylusContext::destroy(JNIEnv * env)
	{
		listeners.clear();

		manager = nullptr;

		jni::JavaContext::destroy(env);
	}
}
