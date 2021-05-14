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

#ifndef STYLUS_UTILS_H_
#define STYLUS_UTILS_H_

#include "JavaRef.h"

#include <jni.h>

long GetJavaWindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window);
long GetJavaAwtWindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window);
long GetJavaFx9WindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window);
long GetComposeWindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window);

#endif
