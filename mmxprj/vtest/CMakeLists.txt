cmake_minimum_required(VERSION 2.8)

# имя проекта
project(vtest)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)

# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set ( SOURCE_FILES
    main.cpp
)

set ( PRIVATE_HEADERS

)

set ( PUBLIC_HEADERS
)

include_directories(

)

link_directories(
#				../mmxprj/mmxlib/build/DEBUG
)

find_library (	MMX_LIBRARY
				mmx
				../mmxprj
)

set( ADDITIONAL_LIBRARIES	
	pcm
)

add_subdirectory(libpcm)

add_executable(${PROJECT_NAME}
${PUBLIC_HEADERS}
${PRIVATE_HEADERS}
${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES} ${MMX_LIBRARY})

