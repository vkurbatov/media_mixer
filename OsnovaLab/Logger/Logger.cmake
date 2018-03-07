set(OL_LOGGER_DIR
	Logger
)

set(OL_LOGGER_CLASSES
	FileLoggerFactory
	ScreenLoggerFactory
	StandartDeviceLogger
	TitleTimeStampLogger
)

set (OL_LOGGER_INTERFACES
	AbstractLoggerFactory
	Logger
)

set (OL_LOGGER_SOURCES
)

set (OL_LOGGER_HEADERS
)

foreach (class ${OL_LOGGER_CLASSES})
	LIST (APPEND OL_LOGGER_SOURCES ${OL_LOGGER_DIR}/${class}.cpp)
	LIST (APPEND OL_LOGGER_HEADERS ${OL_LOGGER_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_LOGGER_INTERFACES})
	LIST (APPEND OL_LOGGER_HEADERS ${OL_LOGGER_DIR}/${interface}.h)
endforeach()



