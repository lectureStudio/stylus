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

#import "CocoaStylusManager.h"
#import "CocoaStylusDevice.h"
#import "CocoaStylusHook.h"

#include <sstream>

namespace stylus
{
	CocoaStylusManager::CocoaStylusManager() :
		StylusManager(),
		stylusContext(std::make_shared<CocoaStylusContext>(this))
	{
		@try {
			[NSApplication setStylusContext:stylusContext];
		}
		@catch (NSException * e) {
			throw e;
		}

		enumerateDevices();
	}

	void CocoaStylusManager::enumerateDevices()
	{
		CFMutableArrayRef matchingArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

		CFMutableDictionaryRef dict = createDeviceDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Mouse);
		if (dict) {
			CFArrayAppendValue(matchingArray, dict);
			CFRelease(dict);
		}

		dict = createDeviceDictionary(kHIDPage_Digitizer, kHIDUsage_Dig_Digitizer);
		if (dict) {
			CFArrayAppendValue(matchingArray, dict);
			CFRelease(dict);
		}

		dict = createDeviceDictionary(kHIDPage_Digitizer, kHIDUsage_Dig_TouchPad);
		if (dict) {
			CFArrayAppendValue(matchingArray, dict);
			CFRelease(dict);
		}

		IOHIDManagerRef hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
		IOHIDManagerSetDeviceMatchingMultiple(hidManager, matchingArray);
		IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
		CFSetRef devices = IOHIDManagerCopyDevices(hidManager);
		CFIndex deviceCount = devices == nullptr ? 0 : CFSetGetCount(devices);

		if (deviceCount > 0) {
			const IOHIDDeviceRef * deviceArray = new IOHIDDeviceRef[deviceCount];
			const IOHIDDeviceRef * current;

			CFSetGetValues(devices, (const void **)deviceArray);

			for (current = deviceArray; current < deviceArray + deviceCount; current++) {
				readDevice(*current);
			}

			CFRelease(devices);
			delete []deviceArray;
		}

		IOHIDManagerClose(hidManager, kIOHIDOptionsTypeNone);
	}

	void CocoaStylusManager::readDevice(const IOHIDDeviceRef device)
	{
		if (CFGetTypeID(device) != IOHIDDeviceGetTypeID()) {
			return;
		}

		int usagePage = 0;
		int usage = 0;

		CFTypeRef type = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDDeviceUsagePageKey));

		if (type) {
			if (CFNumberGetTypeID() == CFGetTypeID(type)) {
				CFNumberGetValue((CFNumberRef) type, kCFNumberSInt32Type, &usagePage);
			}
			CFRelease(type);
		}

		type = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDDeviceUsageKey));

		if (type) {
			if (CFNumberGetTypeID() == CFGetTypeID(type)) {
				CFNumberGetValue((CFNumberRef) type, kCFNumberSInt32Type, &usage);
			}
			CFRelease(type);
		}

		if (!usage || !usagePage) {
			type = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDPrimaryUsagePageKey));

			if (type) {
				if (CFNumberGetTypeID() == CFGetTypeID(type)) {
					CFNumberGetValue((CFNumberRef) type, kCFNumberSInt32Type, &usagePage);
				}
				CFRelease(type);
			}

			type = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDPrimaryUsageKey));

			if (type) {
				if (CFNumberGetTypeID() == CFGetTypeID(type)) {
					CFNumberGetValue((CFNumberRef) type, kCFNumberSInt32Type, &usage);
				}
				CFRelease(type);
			}
		}

		if (!usage || !usagePage) {
			return;
		}

		if (usagePage == kHIDPage_GenericDesktop) {
			switch (usage) {
				case 1 << 1:
					createDevice(device);
					break;
				default:
					break;
			}
		}
		else if (usagePage == kHIDPage_Digitizer) {
			switch (usage) {
				case 1 << 3:
				case 1 << 4:
					createDevice(device);
					break;
				default:
					break;
			}
		}
	}

	void CocoaStylusManager::createDevice(const IOHIDDeviceRef device)
	{
		CFStringRef manufacturer = (CFStringRef) IOHIDDeviceGetProperty(device, CFSTR(kIOHIDManufacturerKey));
		CFStringRef productName = (CFStringRef) IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
		std::stringstream nameStream;
		char buffer[1024];

		if (manufacturer != nullptr) {
			CFStringGetCString(manufacturer, buffer, sizeof(buffer), kCFStringEncodingUTF8);
			nameStream << buffer;
			CFRelease(manufacturer);
		}
		if (productName != nullptr) {
			CFStringGetCString(productName, buffer, sizeof(buffer), kCFStringEncodingUTF8);
			nameStream << " (" << buffer << ")";
			CFRelease(productName);
		}

		stylusDevices.insertDevice(std::make_shared<CocoaStylusDevice>(nameStream.str()));
	}

	CFMutableDictionaryRef CocoaStylusManager::createDeviceDictionary(UInt32 pUsagePage, UInt32 pUsage)
	{
		CFMutableDictionaryRef matchDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFNumberRef usagePage = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &pUsagePage);
		CFNumberRef usage = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &pUsage);

		if (matchDict && usagePage && usage) {
			CFDictionarySetValue(matchDict, CFSTR(kIOHIDDeviceUsagePageKey), usagePage);
			CFDictionarySetValue(matchDict, CFSTR(kIOHIDDeviceUsageKey), usage);
			CFRelease(usagePage);
			CFRelease(usage);
			return matchDict;
		}

		return 0;
	}
}
