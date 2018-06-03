project(mmxprj)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_BUILD_TYPE DEBUG)

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

if (BUILD_DEBUG)
	add_definitions(-DDEBUG)
endif()

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


#add_custom_target(${PROJECT_NAME}_copy_bin
#				COMMAND ${CMAKE_COMMAND} -E copy /home/vkurbatov/1.txt /home/vkurbatov/2.txt
#				)

#)
