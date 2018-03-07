set(OL_UTILS_DIR
	Utils
)

set(OL_UTILS_CLASSES
	Strings/StringUtils
	Time/TimeUtils
)

set (OL_UTILS_HEADERS
	
)

set(OL_UTILS_SOURCES)
set(OL_UTILS_HEADERS)

foreach (class ${OL_UTILS_CLASSES})
	LIST (APPEND OL_UTILS_SOURCES ${OL_UTILS_DIR}/${class}.cpp)
	LIST (APPEND OL_UTILS_HEADERS ${OL_UTILS_DIR}/${class}.h)
endforeach()

