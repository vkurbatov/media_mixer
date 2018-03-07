set(OL_SOCKET_DIR
	Sockets
)

set(OL_SOCKET_CLASSES
	socket_wrp
	Socket
	AbstractChannel
	AbstractConnection
	Select
	RawChannel
	RawConnection
	RawClient
	RawConnectionString
#	SocketDispatcher
	ServerDispatcher
	IP4/IP4Utils
	IP4/SocketBuilder
	IP4/IP4AbstractChannel
	IP4/IP4ConnectionString
	IP4/UdpServerChannel
	IP4/UdpServerConnection
	IP4/UdpClient
	IP4/TcpServerChannel
	IP4/TcpServerConnection
	IP4/TcpClientChannel
	IP4/TcpClientConnection
	IP4/TcpClient
)

set (OL_SOCKET_INTERFACES
	SocketTypes
	Channel
	Connection
	Client
	ConnectionString
	ISelect
	IAsyncCall
)

set (OL_SOCKET_SOURCES
)

set (OL_SOCKET_HEADERS
)



foreach (class ${OL_SOCKET_CLASSES})
	LIST (APPEND OL_SOCKET_SOURCES ${OL_SOCKET_DIR}/${class}.cpp)
	LIST (APPEND OL_SOCKET_HEADERS ${OL_SOCKET_DIR}/${class}.h)
endforeach()

foreach (interface ${OL_SOCKET_INTERFACES})
	LIST (APPEND OL_SOCKET_HEADERS ${OL_SOCKET_DIR}/${interface}.h)
endforeach()



