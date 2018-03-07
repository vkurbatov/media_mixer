set(OL_IO_DIR
	IO
)

set(OL_IO_CLASSES
	AbstractDeviceIO
	FileIO
	ScreenIO
)

set (OL_IO_INTERFACES
	DeviceIO
)

set (OL_IO_SOURCES
)

set (OL_IO_HEADERS
)



foreach (class ${OL_IO_CLASSES})
	LIST (APPEND OL_IO_SOURCES ${OL_IO_DIR}/${class}.cpp)
	LIST (APPEND OL_IO_HEADERS ${OL_IO_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_IO_INTERFACES})
	LIST (APPEND OL_IO_HEADERS ${OL_IO_DIR}/${interface}.h)
endforeach()



