set(OL_THREAD_DIR
	Thread
)

set(OL_THREAD_CLASSES
	StdMutex
	StdConditionVariable
	StdThread
	StdTask
	StdTaskPool
	StdSignal
	StdSlot
	UniqueLock
)

set (OL_THREAD_INTERFACES
	Mutex
	ConditionVariable
	Thread
	ThreadTypes
	Task
	TaskPool
	Signal
	Slot
)

set (OL_THREAD_SOURCES
)

set (OL_THREAD_HEADERS
)



foreach (class ${OL_THREAD_CLASSES})
	LIST (APPEND OL_THREAD_SOURCES ${OL_THREAD_DIR}/${class}.cpp)
	LIST (APPEND OL_THREAD_HEADERS ${OL_THREAD_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_THREAD_INTERFACES})
	LIST (APPEND OL_THREAD_HEADERS ${OL_THREAD_DIR}/${interface}.h)
endforeach()



