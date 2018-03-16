project(mmxprj)
cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(
		mmxlistener
		mmxlib
)

add_subdirectory(
		mmxlistener
)

add_subdirectory(
		mmxlib
)
