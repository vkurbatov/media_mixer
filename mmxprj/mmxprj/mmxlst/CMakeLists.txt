cmake_minimum_required(VERSION 2.8)

# имя проекта
project(mmxlst)


# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set ( SOURCE_FILES
    main.cpp
#	_listener.cpp
	listener.cpp
#	direction.cpp
#	create_dir.cpp
#	destroy_dir.cpp
#	process_dir.cpp
#	dispatch.cpp
#	channel.cpp
#	direction.cpp
#	packet.cpp
)

set ( PRIVATE_HEADERS
	defines.h
#	direction.h
	listener.h
#	channel.h
#	direction.h
#	packet.h
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/../../bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}
					COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/../../bin/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES})

