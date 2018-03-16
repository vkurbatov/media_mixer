set(OL_TOOLS_DIR
	Tools
)

set(OL_TOOLS_CLASSES
	Net/Direction
	Net/PacketQueue
)

set (OL_TOOLS_INTERFACES
	Net/IDirection
)

set (OL_TOOLS_SOURCES
)

set (OL_TOOLS_HEADERS
)



foreach (class ${OL_TOOLS_CLASSES})
	LIST (APPEND OL_TOOLS_SOURCES ${OL_TOOLS_DIR}/${class}.cpp)
	LIST (APPEND OL_TOOLS_HEADERS ${OL_TOOLS_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_TOOLS_INTERFACES})
	LIST (APPEND OL_TOOLS_HEADERS ${OL_TOOLS_DIR}/${interface}.h)
endforeach()



