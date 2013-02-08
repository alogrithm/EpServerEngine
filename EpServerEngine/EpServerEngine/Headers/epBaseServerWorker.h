/*! 
@file epBaseServerWorker.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epserverengine>
@date February 13, 2012
@brief Base Worker Interface
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

An Interface for Base Server Worker.

*/
#ifndef __EP_BASE_SERVER_WORKER_H__
#define __EP_BASE_SERVER_WORKER_H__

#include "epServerEngine.h"
#include "epPacket.h"
#include "epBaseServerSendObject.h"
#include "epBasePacketParser.h"
#include "epServerConf.h"
#include "epParserList.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <vector>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

namespace epse
{
	/*! 
	@class BaseServerWorker epBaseServerWorker.h
	@brief A class for Base Server Worker.
	*/
	class EP_SERVER_ENGINE BaseServerWorker:public BaseServerSendObject
	{
	public:
		/*!
		Default Constructor

		Initializes the Worker
		@param[in] waitTimeMilliSec wait time for Worker Thread to terminate
		@param[in] lockPolicyType The lock policy
		*/
		BaseServerWorker(unsigned int waitTimeMilliSec=WAITTIME_INIFINITE,epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the BaseServerWorker
		@param[in] b the second object
		*/
		BaseServerWorker(const BaseServerWorker& b);

		/*!
		Default Destructor

		Destroy the Worker
		*/
		virtual ~BaseServerWorker();

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		*/
		BaseServerWorker & operator=(const BaseServerWorker&b)
		{
			if(this!=&b)
			{
				epl::LockObj lock(m_sendLock);
				BaseServerSendObject::operator =(b);
// 				m_clientSocket=b.m_clientSocket;
// 
// 				if(m_parserList)
// 					m_parserList->ReleaseObj();
// 				m_parserList=m_parserList;
// 				if(m_parserList)
// 					m_parserList->RetainObj();
			}
			return *this;
		}	

		/*!
		Send the packet to the client
		@param[in] packet the packet to be sent
		@return sent byte size
		*/
		virtual int Send(const Packet &packet);

		/*!
		Get Packet Parser List
		@return the list of the packet parser
		*/
		vector<BaseServerObject*> GetPacketParserList() const;

		/*!
		Check if the connection is alive
		@return true if the connection is alive otherwise false
		*/
		bool IsConnectionAlive() const;

		/*!
		Kill the connection
		*/
		void KillConnection();

	protected:
		/*!
		Return the new packet parser
		@remark Sub-class should implement this to create new parser.
		@remark Client will automatically release this parser.
		@return the new packet parser
		*/
		virtual BasePacketParser* createNewPacketParser()=0;



	private:	
		friend class BaseServer;
		/*!
		Set Parser List for current worker
		@param[in] parserList the parser list to set
		*/
		void setParserList(ParserList *parserList);
	
	
	private:
		/*!
		Actually Kill the connection
		@param[in] fromInternal flag to check if the call is from internal or not
		*/
		void killConnection(bool fromInternal);

		/*!
		thread loop function
		*/
		virtual void execute();

		/*!
		Receive the packet from the client
		@remark  Subclasses must implement this
		@param[out] packet the packet received
		@return received byte size
		*/
		int receive(Packet &packet);
	
		/*!
		Set the argument for the base server worker thread.
		@param[in] clientSocket The client socket from server.
		*/
		void setClientSocket(const SOCKET& clientSocket );


		/// client socket
		SOCKET m_clientSocket;

		/// send lock
		epl::BaseLock *m_sendLock;

		/// kill connection lock
		epl::BaseLock *m_killConnectionLock;
		
		/// Lock Policy
		epl::LockPolicy m_lockPolicy;

		/// Temp Packet;
		Packet m_recvSizePacket;

		/// parser thread list
		ParserList *m_parserList;
	};

}

#endif //__EP_BASE_SERVER_WORKER_H__