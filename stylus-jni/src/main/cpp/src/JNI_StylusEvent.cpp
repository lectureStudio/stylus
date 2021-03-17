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

#include "JNI_StylusEvent.h"
#include "JNI_Stylus.h"
#include "JavaClasses.h"
#include "JavaEnums.h"

namespace stylus
{
	namespace JNI_StylusEvent
	{
		jni::JavaLocalRef<jobject> toJava(JNIEnv * env, const PStylusEvent & event)
		{
			const auto javaClass = jni::JavaClasses::get<JavaStylusEventClass>(env);

			const StylusAxesData & axesData = event->axesData;

			jsize dataSize = static_cast<jsize>(axesData.size());

			jobject jButton = jni::JavaEnums::toJava(env, event->button).release();
			jobject jCursor = jni::JavaEnums::toJava(env, event->cursor).release();

			jdoubleArray jAxesData = env->NewDoubleArray(dataSize);

			if (jAxesData != nullptr) {
				env->SetDoubleArrayRegion(jAxesData, 0, dataSize, axesData.getData());
			}

			jobject jEvent = env->NewObject(javaClass->cls, javaClass->ctor, jButton, jCursor, jAxesData);

			return jni::JavaLocalRef<jobject>(env, jEvent);
		}

		JavaStylusEventClass::JavaStylusEventClass(JNIEnv * env)
		{
			cls = FindClass(env, PKG "StylusEvent");

			ctor = GetMethod(env, cls, "<init>", "(L" PKG "StylusButton;L" PKG "StylusCursor;[D)V");
		}
	}
}
