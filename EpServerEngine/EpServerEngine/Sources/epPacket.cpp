/*! 
Packet for the EpServerEngine
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
*/
#include "epPacket.h"

using namespace epse;

Packet::Packet(const void *packet, unsigned int byteSize, bool shouldAllocate, epl::LockPolicy lockPolicyType):SmartObject(lockPolicyType)
{
	m_packet=NULL;
	m_packetSize=0;
	m_isAllocated=shouldAllocate;
	if(shouldAllocate)
	{
		if(byteSize>0)
		{
			m_packet=EP_NEW char[byteSize];
			if(packet)
				epl::System::Memcpy(m_packet,packet,byteSize);
			else
				epl::System::Memset(m_packet,0,byteSize);
			m_packetSize=byteSize;
		}
	}
	else
	{
		m_packet=reinterpret_cast<char*>(const_cast<void*>(packet));
		m_packetSize=byteSize;
	}
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case epl::LOCK_POLICY_CRITICALSECTION:
		m_packetLock=EP_NEW epl::CriticalSectionEx();
		break;
	case epl::LOCK_POLICY_MUTEX:
		m_packetLock=EP_NEW epl::Mutex();
		break;
	case epl::LOCK_POLICY_NONE:
		m_packetLock=EP_NEW epl::NoLock();
		break;
	default:
		m_packetLock=NULL;
		break;
	}
}

Packet::Packet(const Packet& b):SmartObject(b)
{
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case epl::LOCK_POLICY_CRITICALSECTION:
		m_packetLock=EP_NEW epl::CriticalSectionEx();
		break;
	case epl::LOCK_POLICY_MUTEX:
		m_packetLock=EP_NEW epl::Mutex();
		break;
	case epl::LOCK_POLICY_NONE:
		m_packetLock=EP_NEW epl::NoLock();
		break;
	default:
		m_packetLock=NULL;
		break;
	}

	LockObj lock(b.m_packetLock);
	m_packet=NULL;
	if(b.m_isAllocated)
	{
		if(b.m_packetSize>0)
		{
			m_packet=EP_NEW char[b.m_packetSize];
			epl::System::Memcpy(m_packet,b.m_packet,b.m_packetSize);
		}
		m_packetSize=b.m_packetSize;
	}
	else
	{
		m_packet=b.m_packet;
		m_packetSize=b.m_packetSize;
	}
	m_isAllocated=b.m_isAllocated;
	
}
Packet & Packet::operator=(const Packet&b)
{
	if(this!=&b)
	{
		resetPacket();

		SmartObject::operator =(b);

		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case epl::LOCK_POLICY_CRITICALSECTION:
			m_packetLock=EP_NEW epl::CriticalSectionEx();
			break;
		case epl::LOCK_POLICY_MUTEX:
			m_packetLock=EP_NEW epl::Mutex();
			break;
		case epl::LOCK_POLICY_NONE:
			m_packetLock=EP_NEW epl::NoLock();
			break;
		default:
			m_packetLock=NULL;
			break;
		}

		LockObj lock(b.m_packetLock);
		m_packet=NULL;
		if(b.m_isAllocated)
		{
			if(b.m_packetSize>0)
			{
				m_packet=EP_NEW char[b.m_packetSize];
				epl::System::Memcpy(m_packet,b.m_packet,b.m_packetSize);
			}
			m_packetSize=b.m_packetSize;
		}
		else
		{
			m_packet=b.m_packet;
			m_packetSize=b.m_packetSize;
		}
		m_isAllocated=b.m_isAllocated;

	}
	return *this;
}
void Packet::resetPacket()
{
	m_packetLock->Lock();
	if(m_isAllocated && m_packet)
	{
		EP_DELETE[] m_packet;
	}
	m_packet=NULL;
	m_packetLock->Unlock();
	if(m_packetLock)
		EP_DELETE m_packetLock;
	m_packetLock=NULL;
}

Packet::~Packet()
{
	resetPacket();
}

unsigned int Packet::GetPacketByteSize() const
{
	return m_packetSize;
}

const char *Packet::GetPacket() const
{
	return m_packet;	
}

void Packet::SetPacket(const void* packet, unsigned int packetByteSize)
{
	epl::LockObj lock(m_packetLock);
	if(m_isAllocated)
	{
		if(m_packet)
			EP_DELETE[] m_packet;
		m_packet=NULL;
		if(packetByteSize>0)
		{
			m_packet=EP_NEW char[packetByteSize];
			EP_ASSERT(m_packet);
		}
		if(packet)
			epl::System::Memcpy(m_packet,packet,packetByteSize);
		else
			epl::System::Memset(m_packet,0,packetByteSize);
		m_packetSize=packetByteSize;

	}
	else
	{
		m_packet=reinterpret_cast<char*>(const_cast<void*>(packet));
		m_packetSize=packetByteSize;
	}
}