cmake_minimum_required(VERSION 2.8)

# имя проекта
project(mmxlistener)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)

# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set ( SOURCE_FILES
    main.cpp
	listener.cpp
	create_dir.cpp
	destroy_dir.cpp
	process_dir.cpp
	dispatch.cpp
	channel.cpp
	direction.cpp
	packet.cpp
)

set ( PRIVATE_HEADERS
	defines.h
	channel.h
	direction.h
	packet.h
)

set ( PUBLIC_HEADERS
#	log/log.h
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

target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES})

