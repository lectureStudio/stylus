cmake_minimum_required(VERSION 3.13)
project(stylus-cocoa)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
	INTERFACE
		include/CocoaStylusContext.h
		include/CocoaStylusDevice.h
		include/CocoaStylusHook.h
		include/CocoaStylusManager.h
	PRIVATE
		src/CocoaStylusContext.mm
		src/CocoaStylusDevice.mm
		src/CocoaStylusHook.mm
		src/CocoaStylusManager.mm
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
)
