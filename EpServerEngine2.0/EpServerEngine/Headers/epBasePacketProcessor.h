/*! 
@file epBasePacketProcessor.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epserverengine>
@date July 20, 2012
@brief Base Packet Processor Interface
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

An Interface for Base Packet Processor.

*/
#ifndef __EP_BASE_PACKET_PROCESSOR_H__
#define __EP_BASE_PACKET_PROCESSOR_H__

#include "epServerEngine.h"
#include "epPacket.h"
#include "epBaseServerObject.h"
#include "epServerConf.h"

namespace epse
{
	/*! 
	@class BasePacketProcessor epBasePacketProcessor.h
	@brief A class for Base Packet Processor.
	*/
	class EP_SERVER_ENGINE BasePacketProcessor:public BaseServerObject
	{
	public:
		/*!
		Default Constructor

		Initializes the Processor
		@param[in] waitTimeMilliSec the wait time in millisecond for terminating
		@param[in] lockPolicyType The lock policy
		*/
		BasePacketProcessor(unsigned int waitTimeMilliSec=WAITTIME_INIFINITE,epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);

		/*!
		Default Destructor

		Destroy the Processor
		*/
		virtual ~BasePacketProcessor();

	protected:
		/*!
		Default Copy Constructor

		Initializes the BasePacketProcessor
		@param[in] b the second object
		@remark Copy Constructor prohibited
		*/
		BasePacketProcessor(const BasePacketProcessor& b){}

		/*!
		Assignment operator overloading
		@param[in] b the second object
		@return the new copied object
		@remark Copy Operator prohibited
		*/
		BasePacketProcessor & operator=(const BasePacketProcessor&b){return *this;}


	protected:	
		friend class AsyncTcpClient;

		friend class BaseSocket;
		friend class AsyncTcpSocket;
		friend class AsyncUdpSocket;

		/*! 
		@struct PacketPassUnit epBasePacketProcessor.h
		@brief A class for Packet Passing Unit for Packet Processing Thread.
		*/
		struct PacketPassUnit{
			/// BaseServerWorkerEx Object
			BaseServerObject *m_owner;
			/// Packet to parse
			Packet *m_packet;
		};

		/*!
		Set PacketPassUnit
		@param[in] packetPassUnit PacketPassUnit to set
		*/
		void setPacketPassUnit(const PacketPassUnit& packetPassUnit);	
		

		/*!
		thread loop function
		*/
		virtual void execute()=0;


	protected:

        /// Owner
		BaseServerObject *m_owner;

		/// Packet received
		Packet * m_packetReceived;


	};

}

#endif //__EP_BASE_PACKET_PROCESSOR_H__