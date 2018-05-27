project(mmxprj)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_BUILD_TYPE
	DEBUG
#	RELEASE
)

set(BUILD_TYPE_FLAGS

)

if (${CMAKE_BUILD_TYPE} EQUAL DEBUG)
	set(BUILD_TYPE_FLAGS
		"-d"
	)

	set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
endif()

INCLUDE_DIRECTORIES(
		mmxlst
		mmxcore
		mmxlib
		mmxsrv
		mmxtest
)

add_subdirectory(
		mmxlst
)

add_subdirectory(
		mmxcore
)

add_subdirectory(
		mmxlib
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
