cmake_minimum_required(VERSION 3.13)
project(stylus)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

if(APPLE)
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(STYLUS_OS macos)
	add_subdirectory(dependencies/${STYLUS_OS}/cocoa)
elseif(LINUX)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(STYLUS_OS linux)
	add_subdirectory(dependencies/${STYLUS_OS}/x11)
elseif(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "/MT")
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
	set(STYLUS_OS windows)
	add_subdirectory(dependencies/${STYLUS_OS}/rts)
endif()

add_subdirectory(dependencies/jni-voithos)
add_subdirectory(dependencies/core)

get_target_property(JNI_INCLUDES jni-voithos INCLUDE_DIRECTORIES)
get_target_property(CORE_INCLUDES stylus-core INCLUDE_DIRECTORIES)

set_property(
	TARGET stylus-core
	APPEND PROPERTY INCLUDE_DIRECTORIES ${JNI_INCLUDES}
)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
	INTERFACE
		include/JNI_Stylus.h
		include/JNI_StylusContext.h
		include/JNI_StylusDevice.h
		include/JNI_StylusEvent.h
		include/JNI_StylusListener.h
		include/JNI_StylusManager.h
	PRIVATE
		src/JNI_Stylus.cpp
		src/JNI_StylusContext.cpp
		src/JNI_StylusDevice.cpp
		src/JNI_StylusEvent.cpp
		src/JNI_StylusListener.cpp
		src/JNI_StylusManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	include
)

if(APPLE)
	target_compile_options(${PROJECT_NAME} PRIVATE -x objective-c++)
	set(STYLUS_LINK_LIBS -ObjC "-framework AppKit" "-framework Cocoa" "-framework IOKit" "-framework JavaVM" "-framework QuartzCore")
	set(PLATFORM_LIB stylus-cocoa)
elseif(LINUX)
	target_link_directories(${PROJECT_NAME} PRIVATE dependencies/${STYLUS_OS}/lib/x86-64)
	set(STYLUS_LINK_LIBS X11 Xi Xext jawt -static-libgcc -static-libstdc++)
	set(PLATFORM_LIB stylus-x11)
elseif(WIN32)
	target_link_directories(${PROJECT_NAME} PRIVATE dependencies/${STYLUS_OS}/lib/x86-64)
	set(STYLUS_LINK_LIBS jawt setupapi)
	set(PLATFORM_LIB stylus-rts)
endif()

set_property(
	TARGET ${PLATFORM_LIB}
	APPEND PROPERTY INCLUDE_DIRECTORIES ${JNI_INCLUDES} ${CORE_INCLUDES}
)

target_link_libraries(${PROJECT_NAME} ${PLATFORM_LIB} ${STYLUS_LINK_LIBS} jni-voithos stylus-core)

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT Runtime
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT Runtime
)
