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

#include "JNI_StylusDevice.h"
#include "JNI_Stylus.h"
#include "JavaClasses.h"
#include "JavaString.h"

namespace stylus
{
	namespace JNI_StylusDevice
	{
		jni::JavaLocalRef<jobject> toJava(JNIEnv * env, const PStylusDevice & device)
		{
			const auto javaClass = jni::JavaClasses::get<JavaStylusDeviceClass>(env);

			jstring jDeviceName = jni::JavaString::toJava(env, device->getName()).release();
			jobject jDevice = env->NewObject(javaClass->cls, javaClass->ctor, jDeviceName);

			return jni::JavaLocalRef<jobject>(env, jDevice);
		}

		JavaStylusDeviceClass::JavaStylusDeviceClass(JNIEnv * env)
		{
			cls = FindClass(env, PKG "StylusDevice");

			ctor = GetMethod(env, cls, "<init>", "(" STRING_SIG ")V");
		}
	}
}
