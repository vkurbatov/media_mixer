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
	net/select_ex.cpp
	net/portset.cpp
#	межпроцессное взаимодействие
	ipc/pchannel.cpp
#	ipc/msgchannel.cpp
#	ipc/mediachannel.cpp
#	стафферы
	data/packet.cpp
#	staff/ipstaffer.cpp
#	staff/udpstaffer.cpp
#	staff/mediapool.cpp
	data/dpwriter.cpp
	data/dpreader.cpp
	data/datapacket.cpp
#	склейка пакетов
	sniffers/ipsniffer.cpp
	sniffers/ippacket.cpp
	sniffers/ippacketwrapper.cpp
	sniffers/ippacketpool.cpp
	sniffers/bitset.cpp
	sniffers/dpsniffer.cpp
	sniffers/rtppacketwrapper.cpp
	sniffers/sangomasniffer.cpp
#	плюшки
#	tools/adisp.cpp
	tools/deffwriter.cpp
	tools/timer.cpp
	tools/iosockadp.cpp
	tools/deffersock.cpp
	tools/pipe_input_channel.cpp
	tools/pipe_output_channel.cpp
	tools/pipe_output_channel_pool.cpp
	tools/sangoma_server.cpp
	tools/sangoma_client.cpp
	tools/orm_server.cpp
	tools/orm_client.cpp
	tools/sangoma_media_client.cpp
	tools/template_tcp_server.tpp
#	медиа
	media/sorm.cpp
	media/mediastream.cpp
	media/sormpool.cpp
	media/mediapool.cpp
	media/jitterbuffer.cpp
	media/sample.cpp
	media/codecs/audio/pcma.cpp
	media/codecs/audio/mixer.cpp
#	media/codecs/audio/audiocodec.cpp
)

set ( PRIVATE_HEADERS
#	логи
	logs/log.h
#	сеть
	net/net.h
	net/socket.h
	net/select.h
	net/select_ex.h
	net/portset.h
#	заголовки
	headers/ip.h
	headers/udp.h
	headers/msg.h
	headers/media.h
	headers/datapack.h
	headers/si.h
	headers/order645_2.h
	headers/rtp.h
#	межпроцессное взаимодействие
	ipc/io.h
	ipc/ichannel.h
	ipc/pchannel.h
#	ipc/mediachannel.h
#	стафферы
	data/packet.h
#	staff/istaffer.h
#	staff/ipstaffer.h
#	staff/udpstaffer.h
#	staff/mediapool.h
	data/idpheader.h
	data/idpreader.h
	data/idpwriter.h
	data/dpwriter.h
	data/dpreader.h
	data/datapacket.h
#	склейка пакетов
	sniffers/istream.h
	sniffers/iippacket.h
	sniffers/ipsniffer.h
	sniffers/ippacket.h
	sniffers/ippacketwrapper.h
	sniffers/ippacketpool.h
	sniffers/bitset.h
	sniffers/dpsniffer.h
	sniffers/irtppacket.h
	sniffers/rtppacketwrapper.h
	sniffers/sangomasniffer.h
#	плюшки
#	tools/adisp.h
	tools/deffwriter.h
	tools/timer.h
	tools/iosockadp.h
	tools/deffersock.h
	tools/ichanneldispatcher.h
	tools/pipe_input_channel.h
	tools/pipe_output_channel.h
	tools/pipe_output_channel_pool.h
	tools/orm_server.h
	tools/orm_client.h
	tools/sangoma_media_client.h
	tools/template_tcp_server.h
#	медиа
	media/ijitter.h
	media/sorm.h
	media/mediastream.h
	media/sormpool.h
	media/mediapool.h
	media/jitterbuffer.h
	media/sample.h
	media/isample.h
	media/codecs/audio/pcma.h
	media/codecs/audio/iaudiocodec.h
	media/codecs/audio/mixer.h
#	media/codecs/audio/audiocodec.cpp

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

