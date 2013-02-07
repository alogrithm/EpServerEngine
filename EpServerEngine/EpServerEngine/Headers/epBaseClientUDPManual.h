/*! 
@file epBaseClientUDPManual.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epserverengine>
@date July 18, 2012
@brief Base UDP Client Manual Interface
@version 1.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for Base UDP Client Manual.

*/
#ifndef __EP_BASE_CLIENT_UDP_H__
#define __EP_BASE_CLIENT_UDP_H__

#include "epServerEngine.h"
#include "epPacket.h"
#include "epBaseServerSendObject.h"
#include "epServerConf.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



using namespace std;


namespace epse{

	/*! 
	@class BaseClientUDPManual epBaseClientUDPManual.h
	@brief A class for Base UDP Client Manual.
	*/
	class EP_SERVER_ENGINE BaseClientUDPManual:public BaseServerSendObject{

	public:
		/*!
		Default Constructor

		Initializes the Client
		@param[in] hostName the hostname string
		@param[in] port the port string
		@param[in] lockPolicyType The lock policy
		*/
		BaseClientUDPManual(const TCHAR * hostName=_T(DEFAULT_HOSTNAME), const TCHAR * port=_T(DEFAULT_PORT),epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseClientUDPManual
		@param[in] b the second object
		*/
		BaseClientUDPManual(const BaseClientUDPManual& b);
		/*!
		Default Destructor

		Destroy the Client
		*/
		virtual ~BaseClientUDPManual();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseClientUDPManual & operator=(const BaseClientUDPManual&b)
		{
			if(this!=&b)
			{				
				epl::LockObj lock(m_generalLock);
				BaseServerSendObject::operator =(b);
				m_port=b.m_port;
				m_hostName=b.m_hostName;
			}
			return *this;
		}

		/*!
		Set the hostname for the server.
		@remark Cannot be changed while connected to server
		@param[in] hostName The hostname to set.
		*/
		void SetHostName(const TCHAR * hostName);

		/*!
		Set the port for the server.
		@remark Cannot be changed while connected to server
		@param[in] port The port to set.
		*/
		void SetPort(const TCHAR * port);

		/*!
		Get the hostname of server
		@return the hostname in string
		*/
		epl::EpTString GetHostName() const;

		/*!
		Get the port number of server
		@return the port number in string
		*/
		epl::EpTString GetPort() const;


		/*!
		Get the maximum packet byte size
		@return the maximum packet byte size
		*/
		unsigned int GetMaxPacketByteSize() const;

		/*!
		Connect to the server
		*/
		bool Connect();

		/*!
		Disconnect from the server
		*/
		void Disconnect();

		/*!
		Check if the connection is established
		@return true if the connection is established otherwise false
		*/
		bool IsConnected() const;

		/*!
		Send the packet to the server
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		virtual int Send(const Packet &packet);

		/*!
		Receive the packet from the server
		@return received packet
		@remark the caller must call ReleaseObj() for Packet to avoid the memory leak.
		@remark if ClientSynchronousPolicy!=CLIENT_SYNC_POLICY_MANUAL then assertion.
		*/
		Packet *Receive();

	
	
	private:
		
		
		/*!
		Receive the packet from the server
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);

		
		/*!
		Clean up the client initialization.
		*/
		void cleanUpClient();

		/*!
		Actually Disconnect from the server
		@param[in] fromInternal flag to check if the call is from internal or not
		*/
		void disconnect(bool fromInternal);

		/// Flag for connection
		bool m_isConnected;
		/// port
		epl::EpString m_port;
		/// hostname
		epl::EpString m_hostName;
		/// connection socket
		SOCKET m_connectSocket;
		/// internal variable
		struct addrinfo *m_result;
		/// internal variable2
		struct addrinfo *m_ptr;
		/// internal variable3
		struct addrinfo m_hints;

		/// send lock
		epl::BaseLock *m_sendLock;
		/// general lock
		epl::BaseLock *m_generalLock;

		/// disconnect lock
		epl::BaseLock *m_disconnectLock;

		/// Lock Policy
		epl::LockPolicy m_lockPolicy;

		/// Maximum UDP Datagram byte size
		unsigned int m_maxPacketSize;

	};
}


#endif //__EP_BASE_CLIENT_UDP_H__