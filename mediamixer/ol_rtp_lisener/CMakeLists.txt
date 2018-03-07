cmake_minimum_required(VERSION 2.8)

# имя проекта
project(ol_rtp_lisener)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)

# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set(OL_DIR
	~/work/OsnovaLab
)

set (OL_TEST_DIR
	~/work/tests
)

include_directories(
		${OL_DIR}/..
		${OL_TEST_DIR}
)


include( ${OL_DIR}/OsnovaLab.cmake )

set( PUBLIC_HEADER
	${OL_TEST_DIR}/test.h
)

set ( SOURCE_FILES
    main.cpp
)


set (OL_TESTS
	test1
)

foreach (test ${OL_TESTS})
	LIST (APPEND SOURCE_FILES ${OL_TEST_DIR}/${test}.cpp)
endforeach()

foreach (class ${OL_SOURCES})
	LIST (APPEND SOURCE_FILES ${OL_DIR}/${class})
endforeach()

foreach (class ${OL_HEADERS})
	LIST (APPEND PUBLIC_HEADER ${OL_DIR}/${class})
endforeach()

set( ADDITIONAL_LIBRARIES
	pthread
)

add_executable(${PROJECT_NAME}
${PUBLIC_HEADER}
${PRIVATE_HEADER}
${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES})

