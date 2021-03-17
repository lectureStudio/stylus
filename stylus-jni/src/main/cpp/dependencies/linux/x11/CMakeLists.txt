cmake_minimum_required(VERSION 3.13)
project(stylus-x11)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
	INTERFACE
		include/X11InputDevice.h
		include/X11MouseDevice.h
		include/X11StylusManager.h
		include/X11TabletDevice.h
		include/X11Utils.h
	PRIVATE
		src/X11InputDevice.cpp
		src/X11MouseDevice.cpp
		src/X11StylusManager.cpp
		src/X11TabletDevice.cpp
		src/X11Utils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
)
