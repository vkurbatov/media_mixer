project(mmxprj)
cmake_minimum_required(VERSION 2.8)
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
