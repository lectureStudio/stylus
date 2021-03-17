/*
 * Copyright (c) 2019, Alex Andres. All rights reserved.
 *
 * Use of this source code is governed by the 3-Clause BSD license that can be
 * found in the LICENSE file in the root of the source tree.
 */

#ifndef JNI_WINDOWS_WIN_UTILS_H_
#define JNI_WINDOWS_WIN_UTILS_H_

#include "Exception.h"

#include <comdef.h>
#include <string>
#include <Windows.h>


#define THROW_IF_FAILED(hr, msg, ...) ThrowIfFailed(hr, msg, __VA_ARGS__)


inline std::string WideStrToStr(LPCWSTR wstr)
{
	int wslen = static_cast<int>(wcslen(wstr));
	int length = WideCharToMultiByte(CP_UTF8, 0, wstr, wslen, nullptr, 0, nullptr, nullptr);
	std::string str(length, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr, wslen, &str[0], length, nullptr, nullptr);

	return str;
}

inline std::string BSTRToStr(BSTR bstr)
{
	const int wslen = SysStringLen(bstr);
	const wchar_t * pstr = reinterpret_cast<wchar_t *>(bstr);

	int len = WideCharToMultiByte(CP_ACP, 0, pstr, wslen, nullptr, 0, nullptr, nullptr);

	std::string str(len, '\0');

	WideCharToMultiByte(CP_ACP, 0, pstr, wslen, &str[0], len, nullptr, nullptr);

	return str;
}

inline void ThrowIfFailed(HRESULT hr, const char * msg, ...) {
	if (FAILED(hr)) {
		char message[256];

		va_list args;
		va_start(args, msg);
		vsnprintf(message, 256, msg, args);
		va_end(args);

		std::string comMessage = _com_error(hr).ErrorMessage();

		throw jni::Exception("%s %s", message, comMessage.c_str());
	}
}

struct GUIDHasher
{
	std::size_t operator()(const GUID & guid) const
	{
		std::size_t hash = guid.Data1;
		hash ^= guid.Data2;
		hash ^= guid.Data3;

		for (unsigned i = 0; i < 8; i++) {
			hash ^= (guid.Data4[i] << i);
		}

		return hash;
	}
};

#endif