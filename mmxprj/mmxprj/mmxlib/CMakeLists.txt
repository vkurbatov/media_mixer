cmake_minimum_required(VERSION 2.8)

# имя проекта
project(mmx)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE DEBUG)

# пути для получаемых бинарных модулей

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} )

# рабочая директория проекта

set ( SOURCE_FILES
#	логи
	logs/log.cpp
#	сеть
#	net/create_sock.cpp
#	net/write_sock.cpp
#	net/read_sock.cpp
#	net/sock_opt.cpp
#	net/close_sock.cpp
	net/socket.cpp
	net/select.cpp
	net/portset.cpp
#	межпроцессное взаимодействие
	ipc/pchannel.cpp
#	ipc/msgchannel.cpp
#	ipc/mediachannel.cpp
#	стафферы
	staff/packet.cpp
	staff/ipstaffer.cpp
	staff/udpstaffer.cpp
	staff/mediapool.cpp
	staff/datapacket.cpp
	sniffers/ipsniffer.cpp
	sniffers/ippacket.cpp
	sniffers/ippacketwrapper.cpp
	sniffers/ippacketpool.cpp
	sniffers/bitset.cpp
#	плюшки
	tools/adisp.cpp
	tools/deffwriter.cpp
	tools/timer.cpp
	tools/iosockadp.cpp
)

set ( PRIVATE_HEADERS
#	логи
	logs/log.h
#	сеть
	net/net.h
	net/socket.h
	net/select.h
	net/portset.h
#	заголовки
	headers/ip.h
	headers/udp.h
	headers/msg.h
	headers/media.h
	headers/datapack.h
#	межпроцессное взаимодействие
	ipc/io.h
	ipc/ichannel.h
	ipc/pchannel.h
#	ipc/mediachannel.h
#	стафферы
	staff/packet.h
	staff/istaffer.h
	staff/ipstaffer.h
	staff/udpstaffer.h
	staff/mediapool.h
	staff/datapacket.h
	sniffers/istream.h
	sniffers/iippacket.h
	sniffers/ipsniffer.h
	sniffers/ippacket.h
	sniffers/ippacketwrapper.h
	sniffers/ippacketpool.h
	sniffers/bitset.h
#	плюшки
	tools/adisp.h
	tools/deffwriter.h
	tools/timer.h
	tools/iosockadp.h

#	имена
	names.h
)

set ( PUBLIC_HEADERS

)

set( ADDITIONAL_LIBRARIES
	pthread
)

add_library( ${PROJECT_NAME} 
	${PUBLIC_HEADERS} 
	${PRIVATE_HEADERS} 
	${SOURCE_FILES} )


target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBRARIES})

