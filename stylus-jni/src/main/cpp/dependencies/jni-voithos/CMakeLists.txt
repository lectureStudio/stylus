cmake_minimum_required(VERSION 3.13)
project(jni-voithos)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
	INTERFACE
		include/Exception.h
		include/JavaArray.h
		include/JavaArrayList.h
		include/JavaBigInteger.h
		include/JavaClasses.h
		include/JavaClass.h
		include/JavaContext.h
		include/JavaDimension.h
		include/JavaEnum.h
		include/JavaEnums.h
		include/JavaError.h
		include/JavaFactories.h
		include/JavaFactory.h
		include/JavaHashMap.h
		include/JavaIOException.h
		include/JavaIterable.h
		include/JavaList.h
		include/JavaMapIterator.h
		include/JavaNullPointerException.h
		include/JavaObject.h
		include/JavaPrimitive.h
		include/JavaRectangle.h
		include/JavaRef.h
		include/JavaRuntimeException.h
		include/JavaString.h
		include/JavaThreadEnv.h
		include/JavaThrowable.h
		include/JavaUtils.h
		include/JavaWrappedException.h
	PRIVATE
		src/Exception.cpp
		src/JavaArrayList.cpp
		src/JavaBigInteger.cpp
		src/JavaContext.cpp
		src/JavaDimension.cpp
		src/JavaHashMap.cpp
		src/JavaIterable.cpp
		src/JavaList.cpp
		src/JavaMapIterator.cpp
		src/JavaObject.cpp
		src/JavaRectangle.cpp
		src/JavaString.cpp
		src/JavaThreadEnv.cpp
		src/JavaThrowable.cpp
		src/JavaUtils.cpp
		src/JavaWrappedException.cpp
)

if(APPLE)
	set(JNI_PLATFORM "darwin")
	set_source_files_properties(src/*.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
elseif(LINUX)
	set(JNI_PLATFORM "linux")
elseif(WIN32)
	set(JNI_PLATFORM "windows")

	target_sources(${PROJECT_NAME}
		INTERFACE
			include/windows/ComInitializer.h
			include/windows/ComPtr.h
			include/windows/WinUtils.h
		PRIVATE
			src/windows/ComInitializer.cpp
	)

	target_include_directories(${PROJECT_NAME} PUBLIC
		include/windows
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
)

# Find the JNI package
find_package(JNI)
if (JNI_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC "${JNI_INCLUDE_DIRS}")
endif()