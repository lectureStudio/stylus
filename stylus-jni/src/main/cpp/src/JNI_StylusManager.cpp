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

#include "JNI_Stylus.h"
#include "JNI_StylusContext.h"
#include "JNI_StylusDevice.h"
#include "JNI_StylusManager.h"
#include "JNI_StylusListener.h"
#include "JavaContext.h"
#include "JavaUtils.h"
#include "JavaRef.h"
#include "JavaRuntimeException.h"
#include "StylusException.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <memory>


using namespace stylus;


JNIEXPORT void JNICALL Java_org_lecturestudio_stylus_StylusManager_destroy
(JNIEnv * env, jobject caller)
{
	JNI_StylusContext * context = static_cast<JNI_StylusContext *>(javaContext);
	StylusManager * manager = context->getStylusManager();

	try {
		context->listeners.clear();

		delete manager;
	}
	catch (StylusException & ex) {
		env->Throw(jni::JavaRuntimeException(env, ex.what()));
	}
	catch (...) {
		ThrowCxxJavaException(env);
	}
}

JNIEXPORT void JNICALL Java_org_lecturestudio_stylus_StylusManager_attachStylusListener
(JNIEnv * env, jobject caller, jobject listener, jobject window)
{
	JNI_StylusContext * context = static_cast<JNI_StylusContext *>(javaContext);
	StylusManager * manager = context->getStylusManager();

	try {
		jni::JavaGlobalRef<jobject> listenerRef = jni::JavaGlobalRef<jobject>(env, listener);

		// Connect the Java callback with native listener implementation.
		PJNI_StylusListener stylusListener = std::make_shared<JNI_StylusListener>(env, listenerRef);

		// Keep a reference to the listener.
		SetHandle(env, listener, stylusListener.get());

		manager->attachStylusListener(jni::JavaLocalRef<jobject>(env, window), stylusListener);

		jlong ptr = GetHandleLong<StylusManager>(env, listener);
        context->listeners[ptr] = stylusListener;
	}
	catch (StylusException & ex) {
		env->Throw(jni::JavaRuntimeException(env, ex.what()));
	}
	catch (...) {
		ThrowCxxJavaException(env);
	}
}

JNIEXPORT void JNICALL Java_org_lecturestudio_stylus_StylusManager_detachStylusListener
(JNIEnv * env, jobject caller, jobject listener, jobject window)
{
	JNI_StylusContext * context = static_cast<JNI_StylusContext *>(javaContext);
	StylusManager * manager = context->getStylusManager();

	try {
		jlong ptr = GetHandleLong<StylusManager>(env, listener);
		auto found = context->listeners.find(ptr);

		if (found != context->listeners.end()) {
			PJNI_StylusListener stylusListener = found->second;
			manager->detachStylusListener(jni::JavaLocalRef<jobject>(env, window), stylusListener);
			context->listeners.erase(found);
		}
	}
	catch (StylusException & ex) {
		env->Throw(jni::JavaRuntimeException(env, ex.what()));
	}
	catch (...) {
		ThrowCxxJavaException(env);
	}
}

JNIEXPORT void JNICALL Java_org_lecturestudio_stylus_StylusManager_enableStylusListener
(JNIEnv * env, jobject caller, jobject listener, jboolean enable)
{
	JNI_StylusContext * context = static_cast<JNI_StylusContext *>(javaContext);

	try {
		jlong ptr = GetHandleLong<StylusManager>(env, listener);
		auto found = context->listeners.find(ptr);

		if (found != context->listeners.end()) {
			PJNI_StylusListener stylusListener = found->second;
			stylusListener->setEnabled(enable);
		}
	}
	catch (StylusException & ex) {
		env->Throw(jni::JavaRuntimeException(env, ex.what()));
	}
	catch (...) {
		ThrowCxxJavaException(env);
	}
}

JNIEXPORT jobject JNICALL Java_org_lecturestudio_stylus_StylusManager_getDevices(JNIEnv * env, jobject caller)
{
	JNI_StylusContext * context = static_cast<JNI_StylusContext *>(javaContext);
	StylusManager * manager = context->getStylusManager();

	jobjectArray deviceArray = nullptr;

	try {
		std::set<PStylusDevice > devices = manager->getDevices();
		jsize count = static_cast<jsize>(devices.size());
		int index = 0;

		const auto javaDeviceClass = jni::JavaClasses::get<JNI_StylusDevice::JavaStylusDeviceClass>(env);
		deviceArray = env->NewObjectArray(count, javaDeviceClass->cls, nullptr);

		for (const PStylusDevice & device : devices) {
			jobject inputDevice = JNI_StylusDevice::toJava(env, std::static_pointer_cast<StylusDevice>(device)).release();

			env->SetObjectArrayElement(deviceArray, index++, inputDevice);
		}
	}
	catch (StylusException & ex) {
		env->Throw(jni::JavaRuntimeException(env, ex.what()));
	}
	catch (...) {
		ThrowCxxJavaException(env);
	}

	return deviceArray;
}
