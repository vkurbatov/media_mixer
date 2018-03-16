cmake_minimum_required(VERSION 2.8)

# имя проекта
project(mmx)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)

# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set ( SOURCE_FILES
	logs/log.cpp
	net/create_sock.cpp
	net/write_sock.cpp
	net/read_sock.cpp
	net/sock_opt.cpp
	net/close_sock.cpp
)

set ( PRIVATE_HEADERS
	logs/log.h
	net/net.h
	headers/ip.h
	headers/udp.h
	names.h
)

set ( PUBLIC_HEADERS

)

set( ADDITIONAL_LIBRARIES
	pthread
)

add_library( ${PROJECT_NAME} 
	${PUBLIC_HEADERS} 
	${PRIVATE_HEADERS} 
	${SOURCE_FILES} )


target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES})

