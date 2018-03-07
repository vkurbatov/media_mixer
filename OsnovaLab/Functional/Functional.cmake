set(OL_FUNCTIONAL_DIR
	Functional
)

set(OL_FUNCTIONAL_CLASSES
)

set (OL_FUNCTIONAL_INTERFACES
	Caller
	StdCaller
)

set (OL_FUNCTIONAL_SOURCES
)

set (OL_FUNCTIONAL_HEADERS
)


foreach (class ${OL_FUNCTIONAL_CLASSES})
	LIST (APPEND OL_FUNCTIONAL_SOURCES ${OL_FUNCTIONAL_DIR}/${class}.cpp)
	LIST (APPEND OL_FUNCTIONAL_HEADERS ${OL_FUNCTIONAL_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_FUNCTIONAL_INTERFACES})
	LIST (APPEND OL_FUNCTIONAL_HEADERS ${OL_FUNCTIONAL_DIR}/${interface}.h)
endforeach()



