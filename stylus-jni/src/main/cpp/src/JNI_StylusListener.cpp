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

#include "JNI_StylusListener.h"
#include "JNI_StylusContext.h"
#include "JNI_StylusEvent.h"
#include "JNI_Stylus.h"
#include "JavaUtils.h"

namespace stylus
{
	JNI_StylusListener::JNI_StylusListener(JNIEnv * env, const jni::JavaGlobalRef<jobject> & listener) :
		jListener(listener),
		enabled(false),
		javaClass(jni::JavaClasses::get<JavaStylusListenerClass>(env))
	{
	}

	const jboolean & JNI_StylusListener::isEnabled()
	{
		return enabled;
	}

	void JNI_StylusListener::setEnabled(jboolean enabled)
	{
		this->enabled = enabled;
	}

	void JNI_StylusListener::onCursorChange(PStylusEvent event)
	{
		if (!enabled) {
			return;
		}

		JNIEnv * env = AttachCurrentThread();

		jobject jEvent = JNI_StylusEvent::toJava(env, event).release();

		env->CallVoidMethod(jListener, javaClass->onCursorChange, jEvent);
	}

	void JNI_StylusListener::onCursorMove(PStylusEvent event)
	{
		if (!enabled) {
			return;
		}

		JNIEnv * env = AttachCurrentThread();

		jobject jEvent = JNI_StylusEvent::toJava(env, event).release();

		env->CallVoidMethod(jListener, javaClass->onCursorMove, jEvent);
	}

	void JNI_StylusListener::onButtonDown(PStylusEvent event)
	{
		if (!enabled) {
			return;
		}

		JNIEnv * env = AttachCurrentThread();

		jobject jEvent = JNI_StylusEvent::toJava(env, event).release();

		env->CallVoidMethod(jListener, javaClass->onButtonDown, jEvent);
	}

	void JNI_StylusListener::onButtonUp(PStylusEvent event)
	{
		if (!enabled) {
			return;
		}

		JNIEnv * env = AttachCurrentThread();

		jobject jEvent = JNI_StylusEvent::toJava(env, event).release();

		env->CallVoidMethod(jListener, javaClass->onButtonUp, jEvent);
	}

	JNI_StylusListener::JavaStylusListenerClass::JavaStylusListenerClass(JNIEnv * env)
	{
		jclass cls = FindClass(env, PKG "StylusListener");

		onCursorChange = GetMethod(env, cls, "onCursorChange", "(L" PKG "StylusEvent;)V");
		onCursorMove = GetMethod(env, cls, "onCursorMove", "(L" PKG "StylusEvent;)V");
		onButtonDown = GetMethod(env, cls, "onButtonDown", "(L" PKG "StylusEvent;)V");
		onButtonUp = GetMethod(env, cls, "onButtonUp", "(L" PKG "StylusEvent;)V");
	}
}
