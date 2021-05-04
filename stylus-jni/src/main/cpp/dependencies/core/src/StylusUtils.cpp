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

#include "StylusUtils.h"
#include <string>

#include <jawt.h>
#include <jawt_md.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <Cocoa/Cocoa.h>
#endif

long GetJavaWindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window)
{
    jclass javaClass = env->GetObjectClass(window);
    jclass javaClassClass = env->FindClass("java/lang/Class");
    jmethodID javaClassNameMethod = env->GetMethodID(javaClassClass, "getName", "()Ljava/lang/String;");

    jstring javaClassNameJString = (jstring)env->CallObjectMethod(javaClass, javaClassNameMethod);
    const char * javaClassNameC = env->GetStringUTFChars(javaClassNameJString, 0);
    //printf("%s\n", javaClassNameC); //for adding new cases

    jlong windowId;
    std::string javaClassName(javaClassNameC);

    if (javaClassName.compare("androidx.compose.desktop.AppWindow") == 0) {
        windowId = GetComposeWindowId(env, window);
    }

    if (javaClassName.compare("javafx.stage.Stage") == 0) {
        windowId = GetJavaFx9WindowId(env, window);
    }

    if (javaClassName.compare("javax.swing.JFrame") == 0) {
        windowId = GetJavaAwtWindowId(env, window);
    }

    if (javaClassName.compare("java.lang.Long") == 0) {
        jclass cls = env->FindClass("java/lang/Long");
        jmethodID longGetLongValue= env->GetMethodID(cls,"longValue","()J");
        windowId = (env)->CallLongMethod(window, longGetLongValue);
    }

    env->ReleaseStringUTFChars(javaClassNameJString, javaClassNameC);

    return static_cast<long>(windowId);
}

long GetJavaAwtWindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window)
{
	long windowId = 0;

	JAWT awt;
	JAWT_DrawingSurface * ds;
	JAWT_DrawingSurfaceInfo * dsi;

	// Get the AWT.
	awt.version = JAWT_VERSION_1_7;
	if (JAWT_GetAWT(env, &awt) == JNI_FALSE) {
		return windowId;
	}

	ds = awt.GetDrawingSurface(env, window);
	if (ds == NULL) {
		return windowId;
	}

	jint lock = ds->Lock(ds);
	if ((lock & JAWT_LOCK_ERROR) != 0) {
		awt.FreeDrawingSurface(ds);
		return windowId;
	}

	dsi = ds->GetDrawingSurfaceInfo(ds);
	if (dsi == nullptr) {
		ds->Unlock(ds);
		awt.FreeDrawingSurface(ds);
		return windowId;
	}

#ifdef _WIN32
	JAWT_Win32DrawingSurfaceInfo * dsiWin32 = reinterpret_cast<JAWT_Win32DrawingSurfaceInfo *>(dsi->platformInfo);
	windowId = HandleToLong(dsiWin32->hwnd);
#endif
#ifdef __linux__
	JAWT_X11DrawingSurfaceInfo * dsiX11 = reinterpret_cast<JAWT_X11DrawingSurfaceInfo *>(dsi->platformInfo);
	windowId = dsiX11->drawable;
#endif
#ifdef __APPLE__
	id<JAWT_SurfaceLayers> surfaceLayers = (id<JAWT_SurfaceLayers>)CFBridgingRelease(dsi->platformInfo);
	CALayer * windowLayer = surfaceLayers.windowLayer;

	windowId = (long) windowLayer;
#endif

	// Free resources.
	ds->FreeDrawingSurfaceInfo(dsi);
	ds->Unlock(ds);
	awt.FreeDrawingSurface(ds);

	return windowId;
}

long GetJavaFx9WindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window)
{
	jclass cls = env->GetObjectClass(window);
	jmethodID method = env->GetMethodID(cls, "getPeer", "()Lcom/sun/javafx/tk/TKStage;");

	if (method == nullptr) {
		// Window.getPeer() not found.
		return 0;
	}

	jobject tkStage = env->CallObjectMethod(window, method);

	if (tkStage == nullptr) {
		// NullPointer tkStage.
		return 0;
	}

	cls = env->GetObjectClass(tkStage);
	method = env->GetMethodID(cls, "getRawHandle", "()J");

	if (method == nullptr) {
		// TkStage.getRawHandle() not found.
		return 0;
	}

	jlong windowId = env->CallLongMethod(tkStage, method);

	return static_cast<long>(windowId);
}

long GetComposeWindowId(JNIEnv * env, const jni::JavaLocalRef<jobject> & window)
{
	jclass cls = env->GetObjectClass(window);
	jmethodID method = env->GetMethodID(cls, "getWindow", "()Landroidx/compose/desktop/ComposeWindow;");

	if (method == nullptr) {
		// Window.getWindow() not found.
		return 0;
	}

	jobject composeWindow = env->CallObjectMethod(window, method);

	if (composeWindow == nullptr) {
		// NullPointer composeWindow.
		return 0;
	}

	cls = env->GetObjectClass(composeWindow);
	method = env->GetMethodID(cls, "getWindowHandle", "()J");

	if (method == nullptr) {
		// composeWindow.getWindowHandle() not found.
		return 0;
	}

	jlong windowId = env->CallLongMethod(composeWindow, method);

	return static_cast<long>(windowId);
}
