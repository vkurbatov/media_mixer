cmake_minimum_required(VERSION 2.8)

# имя проекта
project(mmxtest)


# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set ( SOURCE_FILES
	main.cpp
	test1.cpp
	test2.cpp
	test3.cpp
	test4.cpp
	test5.cpp
	test6.cpp
	test7.cpp
	test8.cpp
	test9.cpp
	test10.cpp
	test11.cpp
	test12.cpp
)

set ( PRIVATE_HEADERS
	defines.h
)

set ( PUBLIC_HEADERS
)

include_directories(
	..
)

set( ADDITIONAL_LIBRARIES
	pthread
	mmx
)

add_executable(${PROJECT_NAME}
${PUBLIC_HEADERS}
${PRIVATE_HEADERS}
${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${BUILD_TYPE_FLAGS} ${ADDITIONAL_LIBRARIES})

