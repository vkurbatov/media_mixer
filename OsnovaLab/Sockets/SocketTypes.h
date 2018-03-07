#ifndef _OL_SOCKET_TYPES_H
#define _OL_SOCKET_TYPES_H

#ifdef WIN32

#include <windows.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in
#include <stdio.h> // close
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#ifndef SOCKET

typedef int SOCKET;

#define SOCKET_ERROR    (-1)
#define SOCKET_INVALID  (~0)

#endif

#endif


namespace OsnovaLab
{
	namespace Sockets
	{
		using default_socket_result_t		= int;
		using default_socket_size_t			= long;
		using default_socket_timeout_t		= int;
		using default_socket_data_t			= void;
		using default_socket_ptr_data_t		= default_socket_data_t*;
		using default_context_t				= void*;
		
		using socket_timeout_t				= default_socket_timeout_t;
		using native_socket_t				= SOCKET;
		using socket_result_t				= default_socket_result_t;
		using socket_size_t					= default_socket_size_t;
		using packet_data_t					= default_socket_ptr_data_t;
		using context_t						= default_context_t;

        static const int DOMAIN_DEFAULT = PF_INET;

		static const socket_timeout_t TIMEOUT_INFINITE	= -1;
		static const socket_timeout_t TIMEOUT_NULL		= 0;

		static const socket_result_t SOCKET_S_SUCCESS	= 0;
		static const socket_result_t SOCKET_E_ERROR		= -1;

		static const socket_size_t SOCKET_SET_SIZE		= FD_SETSIZE;

		static const default_socket_result_t DEFAULT_S_SUCCESS		= 0;
		static const default_socket_result_t DEFAULT_S_FALSE		= 0;
		static const default_socket_result_t DEFAULT_S_DISABLE		= 0;
		static const default_socket_result_t DEFAULT_S_TRUE			= 1;
		static const default_socket_result_t DEFAULT_S_ENABLE		= 1;

		static const default_socket_result_t DEFAULT_E_INVARG		= -1;
		static const default_socket_result_t DEFAULT_E_HANDLE		= -2;
		static const default_socket_result_t DEFAULT_E_NOTIMPL		= -3;
		static const default_socket_result_t DEFAULT_E_IO			= -4;
		static const default_socket_result_t DEFAULT_E_CONFIG		= -5;
		static const default_socket_result_t DEFAULT_E_BUSY			= -6;
		//static const default_socket_result_t DEFAULT_E_EMPTY		= -7;
		static const default_socket_result_t DEFAULT_E_UNBLOCK		= -10;
		static const default_socket_result_t DEFAULT_E_CONNECT		= -20;
		static const default_socket_result_t DEFAULT_E_UNKNOWN		= -30;
		static const default_socket_result_t DEFAULT_E_TIMEOUT		= -100;
        //стартовая точка для кодировки сошибокк системных вызовов
        static const default_socket_result_t DEFAULT_E_SYSCALL		= -1000000;

		enum SocketType : int
		{
			SOCKET_RAW = SOCK_RAW,
			SOCKET_TCP = SOCK_STREAM,
			SOCKET_UDP = SOCK_DGRAM
		};

		enum SocketBlocketType : int
		{
			SOCKET_UNBLOCKED = 0,
			SOCKET_BLOCKED = 1
		};

		enum SocketFragmentationMode : bool
		{
			SOCKET_FRAMENTATION_MODE_OFF = false,
			SOCKET_FRAMENTATION_MODE_ON = true
		};
	
		
		inline bool IsSocketValid(const native_socket_t& socket_handle) { return ((socket_size_t)socket_handle) >= 0; }
		inline bool IsSuccess(const socket_result_t& result) { return (result) >= 0; }
		inline bool IsError(const socket_result_t& result) { return !IsSuccess(result); }

		namespace IP4
		{
			using address_type = unsigned int;
			using address_seg_type = unsigned char;
			using port_type = unsigned short;

			static const address_type SOCKET_ADDRESS_ANY = INADDR_ANY;
			static const address_type SOCKET_ADDRESS_LOOPBACK = INADDR_LOOPBACK;
			static const address_type SOCKET_ADDRESS_BROADCAST = INADDR_BROADCAST;
			static const address_type SOCKET_ADDRESS_NONE = INADDR_NONE;

			struct IP4Address
			{
				union
				{
					address_type	Address;
					address_seg_type	AddressSeg[sizeof(address_type)];
				};
				port_type		Port;
				IP4Address(address_type address = 0, port_type port = 0) :
					Address(address),
					Port(port) {}

				IP4Address(const IP4Address& address) :
					Address(address.Address),
					Port(address.Port) {}
			};

			inline bool IsAddrNone(const IP4Address& addr) { return addr.Address == SOCKET_ADDRESS_NONE; }
			inline bool IsAddrLoppback(const IP4Address& addr) { return addr.Address == SOCKET_ADDRESS_LOOPBACK; }
			inline bool IsAddrBroadcast(const IP4Address& addr) { return addr.Address == SOCKET_ADDRESS_BROADCAST; }
			inline bool IsAddrAny(const IP4Address& addr) { return addr.Address == SOCKET_ADDRESS_ANY; }
			//static const IP4Address IP4_ADDRESS_NONE = IP4Address(SOCKET_ADDRESS_NONE, 0);
		}
	}
}

#endif
