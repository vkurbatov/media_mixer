set(OL_DATA_DIR
	Data
)

set(OL_DATA_CLASSES
#	Packet
	Packet
)

set (OL_DATA_INTERFACES
	IEnumerator
	IEnumerable
	IQueue
	ICollection
	DataTypes
	Queue
)

set (OL_DATA_SOURCES
)

set (OL_DATA_HEADERS
)



foreach (class ${OL_DATA_CLASSES})
	LIST (APPEND OL_DATA_SOURCES ${OL_DATA_DIR}/${class}.cpp)
	LIST (APPEND OL_DATA_HEADERS ${OL_DATA_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_DATA_INTERFACES})
	LIST (APPEND OL_DATA_HEADERS ${OL_DATA_DIR}/${interface}.h)
endforeach()



