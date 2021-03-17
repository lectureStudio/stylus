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

#ifndef JNI_STYLUS_CONTEXT_H
#define JNI_STYLUS_CONTEXT_H

#include "JavaContext.h"
#include "JNI_StylusListener.h"
#include "StylusManager.h"

#include <jni.h>
#include <map>
#include <memory>

namespace stylus
{
	class JNI_StylusContext : public jni::JavaContext
	{
		public:
			JNI_StylusContext(JavaVM * vm);
			~JNI_StylusContext() = default;

			StylusManager * getStylusManager();

			void initialize(JNIEnv * env) override;
			void destroy(JNIEnv * env) override;

		public:
			std::map<jlong, PJNI_StylusListener> listeners;

		private:
			std::unique_ptr<StylusManager> manager;
	};
}

#endif
