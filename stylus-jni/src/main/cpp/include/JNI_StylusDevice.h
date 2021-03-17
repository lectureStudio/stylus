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

#ifndef JNI_STYLUS_DEVICE_H_
#define JNI_STYLUS_DEVICE_H_

#include "JavaClass.h"
#include "JavaRef.h"

#include "StylusDevice.h"

#include <jni.h>

namespace stylus
{
	namespace JNI_StylusDevice
	{
		class JavaStylusDeviceClass : public jni::JavaClass
		{
			public:
				explicit JavaStylusDeviceClass(JNIEnv * env);

				jclass cls;
				jmethodID ctor;
		};

		jni::JavaLocalRef<jobject> toJava(JNIEnv * env, const PStylusDevice & device);
	}
}

#endif
