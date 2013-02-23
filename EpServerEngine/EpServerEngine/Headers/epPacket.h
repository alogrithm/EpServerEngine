/*! 
@file epPacket.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/epserverengine>
@date February 13, 2012
@brief Packet Interface
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

An Interface for Packet.

*/
#ifndef __EP_PACKET_H__
#define __EP_PACKET_H__

#include "epServerEngine.h"

namespace epse{

	/*! 
	@class Packet epPacket.h
	@brief A class for Packet.
	*/
	class EP_SERVER_ENGINE Packet:public epl::SmartObject{

	public:
		/*!
		Default Constructor

		Initializes the Packet
		@param[in] packet packet to copy from
		@param[in] byteSize the byte size of the packet given
		@param[in] shouldAllocate flag for the allocation of memory for itself
		@param[in] lockPolicyType The lock policy
		*/
		Packet(const void *packet=NULL, unsigned int byteSize=0, bool shouldAllocate=true, epl::LockPolicy lockPolicyType=epl::EP_LOCK_POLICY);

		/*!
		Default Copy Constructor

		Initializes the Packet
		@param[in] b the original Packet object
		*/
		Packet(const Packet& b);

		/*!
		Assignment Operator Overloading

		the Packet set as given packet b
		@param[in] b right side of packet
		@return this object
		*/
		Packet & operator=(const Packet&b);

		/*!
		Default Destructor

		Destroy the Packet
		*/
		virtual ~Packet();

		/*!
		Return the currently stored packet byte size
		@return byte size of the holding packet
		*/
		 unsigned int GetPacketByteSize() const;

		/*!
		Get the flag whether memory is allocated or not
		@return true if the memory is allocated by this object, otherwise false
		*/
		bool IsAllocated() const
		{
			return m_isAllocated;
		}

		/*!
		Return the currently holding packet
		@return holding packet
		*/
		const char *GetPacket() const;

		/*!
		Set the packet as given
		@param[in] packet the packet data
		@param[in] packetByteSize the size of packet given
		*/
		void SetPacket(const void* packet, unsigned int packetByteSize);

	private:

		/*!
		Reset Packet
		*/
		void resetPacket();
		/// packet
		char *m_packet;
		/// packet Byte Size
		unsigned int m_packetSize;
		/// flag whether memory is allocated in this object or now
		bool m_isAllocated;
		/// lock
		epl::BaseLock *m_packetLock;
		/// Lock Policy
		epl::LockPolicy m_lockPolicy;
	};
}


#endif //__EP_PACKET_H__