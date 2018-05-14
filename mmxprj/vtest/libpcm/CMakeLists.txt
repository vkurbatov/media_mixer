cmake_minimum_required(VERSION 2.8)

# имя проекта
project(pcm)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)

# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )



# рабочая директория проекта

set ( SOURCE_FILES
	alsa_recorder.cpp
	alsa_player.cpp
	pcm_device.cpp
)

set ( PRIVATE_HEADERS
	alsa_recorder.h
	alsa_player.h
	pcm_device.h
)

set ( PUBLIC_HEADERS

)

include_directories(
	../../mmxprj
)

set( ADDITIONAL_LIBRARIES
	asound
)


add_library( ${PROJECT_NAME} 
	${PUBLIC_HEADERS} 
	${PRIVATE_HEADERS} 
	${SOURCE_FILES} )


target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES})

