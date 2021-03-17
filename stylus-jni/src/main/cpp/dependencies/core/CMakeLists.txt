cmake_minimum_required(VERSION 3.13)
project(stylus-core)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
	INTERFACE
		include/Convert.h
		include/DeviceList.h
		include/Event.h
		include/EventDispatcher.h
		include/Queue.h
		include/Stylus.h
		include/StylusAxesData.h
		include/StylusAxisContext.h
		include/StylusDevice.h
		include/StylusDeviceListener.h
		include/StylusEvent.h
		include/StylusException.h
		include/StylusListener.h
		include/StylusManager.h
		include/StylusUtils.h
	PRIVATE
		src/StylusAxesData.cpp
		src/StylusAxisContext.cpp
		src/StylusDevice.cpp
		src/StylusEvent.cpp
		src/StylusException.cpp
		src/StylusManager.cpp
		src/StylusUtils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
)

if(APPLE)
	target_compile_options(${PROJECT_NAME} PRIVATE -x objective-c++)
endif()
