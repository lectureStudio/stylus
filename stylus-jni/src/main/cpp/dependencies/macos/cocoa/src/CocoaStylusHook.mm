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

#import "CocoaStylusHook.h"
#import "CocoaStylusContext.h"

#import <objc/objc-class.h>


static stylus::PCocoaStylusContext stylusContext;


@implementation NSApplication (CocoaStylusHook)

+ (void) setStylusContext: (stylus::PCocoaStylusContext)context
{
	SEL originalSelector = @selector(sendEvent:);
	SEL swizzledSelector = @selector(sendEventHook:);
	
	[NSApplication swizzleMethod:originalSelector withMethod:swizzledSelector];
	
	stylusContext = context;
}

- (void) sendEventHook: (NSEvent *)event
{
	switch ([event type]) {
		case NSEventTypeTabletProximity:
			stylusContext->onProximityEvent(event);
			break;
			
		case NSEventTypeTabletPoint:
			stylusContext->onPointEvent(event);
			break;
			
		case NSEventTypeMouseMoved:
			stylusContext->onPointEvent(event);
			break;
			
		case NSEventTypeLeftMouseDragged:
		case NSEventTypeRightMouseDragged:
		case NSEventTypeOtherMouseDragged:
			stylusContext->onPointEvent(event);
			break;
			
		case NSEventTypeLeftMouseDown:
		case NSEventTypeRightMouseDown:
		case NSEventTypeOtherMouseDown:
			stylusContext->onButtonPressedEvent(event);
			break;
			
		case NSEventTypeLeftMouseUp:
		case NSEventTypeRightMouseUp:
		case NSEventTypeOtherMouseUp:
			stylusContext->onButtonReleasedEvent(event);
			break;
			
		default:
			break;
	}

	// 'sendEventHook' points to the original NSApplication 'sendEvent' method.
	[self sendEventHook: event];
}

+ (void) swizzleMethod:(SEL)originalSelector withMethod:(SEL)swizzledSelector
{
	Method originalMethod = class_getInstanceMethod(self, originalSelector);

	if (!originalMethod) {
		NSString * reason = [NSString stringWithFormat:@"Method %@::%@ not found.", [self className], NSStringFromSelector(originalSelector)];
		NSException * exception = [NSException exceptionWithName:@"SwizzleException" reason:reason userInfo:nil];
		@throw exception;
	}

	Method swizzledMethod = class_getInstanceMethod(self, swizzledSelector);

	if (!swizzledMethod) {
		NSString * reason = [NSString stringWithFormat:@"Method %@::%@ not found.", [self className], NSStringFromSelector(swizzledSelector)];
		NSException * exception = [NSException exceptionWithName:@"SwizzleException" reason:reason userInfo:nil];
		@throw exception;
	}

	BOOL didAddMethod = class_addMethod(self, originalSelector, method_getImplementation(swizzledMethod), method_getTypeEncoding(swizzledMethod));

	if (didAddMethod) {
		class_replaceMethod(self, swizzledSelector, method_getImplementation(originalMethod), method_getTypeEncoding(originalMethod));
	}
	else {
		method_exchangeImplementations(originalMethod, swizzledMethod);
	}
}

@end
