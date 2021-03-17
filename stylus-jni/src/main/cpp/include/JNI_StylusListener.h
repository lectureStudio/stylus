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

#ifndef JNI_STYLUS_LISTENER_H
#define JNI_STYLUS_LISTENER_H

#include "StylusListener.h"
#include "JavaClass.h"
#include "JavaRef.h"

#include <jni.h>

namespace stylus
{
	class JNI_StylusListener : public StylusListener
	{
		public:
			JNI_StylusListener(JNIEnv * env, const jni::JavaGlobalRef<jobject> & listener);
			~JNI_StylusListener() = default;

			const jboolean & isEnabled();
			void setEnabled(jboolean enabled);

			void onCursorChange(PStylusEvent event);
			void onCursorMove(PStylusEvent event);
			void onButtonDown(PStylusEvent event);
			void onButtonUp(PStylusEvent event);

		private:
			class JavaStylusListenerClass : public jni::JavaClass
			{
				public:
					explicit JavaStylusListenerClass(JNIEnv * env);

					jmethodID onCursorChange;
					jmethodID onCursorMove;
					jmethodID onButtonDown;
					jmethodID onButtonUp;
			};

		private:
			jni::JavaGlobalRef<jobject> jListener;

			/* Indicates whether the listener should receive stylus events. */
			jboolean enabled;

			const std::shared_ptr<JavaStylusListenerClass> javaClass;
	};


	using PJNI_StylusListener = std::shared_ptr<JNI_StylusListener>;
}

#endif
