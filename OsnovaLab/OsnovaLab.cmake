set(OL_CLASSES
	Builder
)

set (OL_HEADERS

)

#**************************************************

foreach (class ${OL_CLASSES})
	LIST (APPEND OL_SOURCES ${class}.cpp)
	LIST (APPEND OL_HEADERS ${class}.h)
endforeach()

#**************************************************

#utils block
include ( ${OL_DIR}/Utils/Utils.cmake )

#utils sources
foreach (utils ${OL_UTILS_SOURCES})
	LIST (APPEND OL_SOURCES ${utils})
endforeach()

#utils headers
foreach (utils ${OL_UTILS_HEADERS})
	LIST (APPEND OL_HEADERS ${utils})
endforeach()


#**************************************************

#functional block
include ( ${OL_DIR}/Functional/Functional.cmake )

#functional sources
foreach (functional ${OL_FUNCTIONAL_SOURCES})
	LIST (APPEND OL_SOURCES ${functional})
endforeach()

#functional headers
foreach (functional ${OL_FUNCTIONAL_HEADERS})
	LIST (APPEND OL_HEADERS ${functional})
endforeach()

#**************************************************

#thread block
include ( ${OL_DIR}/Thread/Thread.cmake )

#thread sources
foreach (thread ${OL_THREAD_SOURCES})
	LIST (APPEND OL_SOURCES ${thread})
endforeach()

#thread headers
foreach (thread ${OL_THREAD_HEADERS})
	LIST (APPEND OL_HEADERS ${thread})
endforeach()

#**************************************************

#IO block
include ( ${OL_DIR}/IO/IO.cmake )

#IO sources
foreach (io ${OL_IO_SOURCES})
	LIST (APPEND OL_SOURCES ${io})
endforeach()

#IO headers
foreach (io ${OL_IO_HEADERS})
	LIST (APPEND OL_HEADERS ${io})
endforeach()

#**************************************************

#logger block
include ( ${OL_DIR}/Logger/Logger.cmake )

#logger sources
foreach (logger ${OL_LOGGER_SOURCES})
	LIST (APPEND OL_SOURCES ${logger})
endforeach()

#logger headers
foreach (logger ${OL_LOGGER_HEADERS})
	LIST (APPEND OL_HEADERS ${logger})
endforeach()

#**************************************************

#data block
include ( ${OL_DIR}/Data/Data.cmake )

#data sources
foreach (data ${OL_DATA_SOURCES})
	LIST (APPEND OL_SOURCES ${data})
endforeach()

#data headers
foreach (data ${OL_DATA_HEADERS})
	LIST (APPEND OL_HEADERS ${data})
endforeach()

#**************************************************

#socket block
include ( ${OL_DIR}/Sockets/Sockets.cmake )

#socket sources
foreach (socket ${OL_SOCKET_SOURCES})
	LIST (APPEND OL_SOURCES ${socket})
endforeach()

#socket headers
foreach (socket ${OL_SOCKET_HEADERS})
	LIST (APPEND OL_HEADERS ${socket})
endforeach()

#**************************************************

#tools block
include ( ${OL_DIR}/Tools/Tools.cmake )

#tools sources
foreach (tool ${OL_TOOLS_SOURCES})
	LIST (APPEND OL_SOURCES ${tool})
endforeach()

#tools headers
foreach (tool ${OL_TOOLS_HEADERS})
	LIST (APPEND OL_HEADERS ${tool})
endforeach()

#**************************************************
