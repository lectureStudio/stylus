cmake_minimum_required(VERSION 3.13)
project(stylus-rts)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
	INTERFACE
		include/RealTimeStylusCursorContext.h
		include/RealTimeStylusDevice.h
		include/RealTimeStylusEventHandler.h
		include/RealTimeStylusManager.h
		include/RealTimeStylusUtils.h
	PRIVATE
		src/RealTimeStylusCursorContext.cpp
		src/RealTimeStylusDevice.cpp
		src/RealTimeStylusEventHandler.cpp
		src/RealTimeStylusManager.cpp
		src/RealTimeStylusUtils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
)
