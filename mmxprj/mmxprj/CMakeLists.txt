project(mmxprj)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_BUILD_TYPE DEBUG)

SET(CMAKE_BUILD_NUMBER 1)
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE
		DEBUG
		)
endif(NOT CMAKE_BUILD_TYPE)

message("Build " ${CMAKE_BUILD_TYPE} " version")

if(NOT CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	set(BUILD_DEBUG 1)
else()
	set(BUILD_DEBUG 0)

endif()

if ($ENV{BUILD_NUMBER})

	SET(CMAKE_BUILD_NUMBER $ENV{BUILD_NUMBER})

endif()

add_definitions("-DBUILD_NUMBER=${CMAKE_BUILD_NUMBER}")

if (BUILD_DEBUG)
	add_definitions(-DDEBUG)
endif()

# для логов
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

INCLUDE_DIRECTORIES(
		mmxlst
		mmxcore
		mmxlib
		mmxsrv
		mmxtest
)

add_subdirectory(
		mmxlib
)

add_subdirectory(
		mmxlst
)

add_subdirectory(
		mmxcore
)

add_subdirectory(
		mmxsrv
)

add_subdirectory(
		mmxtest
)

