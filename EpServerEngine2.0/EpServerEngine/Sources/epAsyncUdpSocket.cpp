/*! 
AsyncUdpSocket for the EpServerEngine
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
#include "epAsyncUdpSocket.h"
#include "epAsyncUdpServer.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epse;
AsyncUdpSocket::AsyncUdpSocket(ServerCallbackInterface *callBackObj,unsigned int waitTimeMilliSec,unsigned int maximumProcessorCount,epl::LockPolicy lockPolicyType): BaseUdpSocket(callBackObj,waitTimeMilliSec,lockPolicyType)
{
	m_maxProcessorCount=maximumProcessorCount;
}

AsyncUdpSocket::~AsyncUdpSocket()
{
	KillConnection();
}
void AsyncUdpSocket::SetMaximumProcessorCount(unsigned int maxProcessorCount)
{
	epl::LockObj lock(m_baseSocketLock);
	m_maxProcessorCount=maxProcessorCount;

}
unsigned int AsyncUdpSocket::GetMaximumProcessorCount() const
{
	epl::LockObj lock(m_baseSocketLock);
	return m_maxProcessorCount;
}


void AsyncUdpSocket::KillConnection()
{
	epl::LockObj lock(m_baseSocketLock);
	if(!IsConnectionAlive())
	{
		return;
	}
	m_threadStopEvent.SetEvent();
	if(GetStatus()==Thread::THREAD_STATUS_SUSPENDED)
		Resume();
	TerminateAfter(m_waitTime);
	m_processorList.Clear();

	m_listLock->Lock();
	Packet *removeElem=NULL;
	while(!m_packetList.empty())
	{
		removeElem=m_packetList.front();
		if(removeElem)
			removeElem->ReleaseObj();
		m_packetList.pop();
	}
	m_listLock->Unlock();

	removeSelfFromContainer();
	m_callBackObj->OnDisconnect(this);
}

void AsyncUdpSocket::killConnection()
{
	if(IsConnectionAlive())
	{

		m_processorList.Clear();
		m_listLock->Lock();
		Packet *removeElem=NULL;
		while(!m_packetList.empty())
		{
			removeElem=m_packetList.front();
			if(removeElem)
				removeElem->ReleaseObj();
			m_packetList.pop();
		}
		m_listLock->Unlock();

		removeSelfFromContainer();
		m_callBackObj->OnDisconnect(this);

	}
}
void AsyncUdpSocket::execute()
{
	unsigned int packetSize=0;

	while(1)
	{
		if(m_threadStopEvent.WaitForEvent(WAITTIME_IGNORE))
		{
			break;
		}
		m_listLock->Lock();
		if(m_packetList.size()==0)
		{
			m_listLock->Unlock();
			Suspend();
			continue;
		}
		Packet *packet= m_packetList.front();
		packetSize=packet->GetPacketByteSize();
		if(packetSize==0)
		{
			m_listLock->Unlock();
			break;
		}
		m_packetList.pop();
		m_listLock->Unlock();

		ServerPacketProcessor::PacketPassUnit passUnit;
		passUnit.m_owner=this;
		passUnit.m_packet=packet;
		ServerPacketProcessor *parser =EP_NEW ServerPacketProcessor(m_callBackObj,m_waitTime,m_lockPolicy);
		parser->setPacketPassUnit(passUnit);
		m_processorList.Push(parser);
		parser->Start();
		packet->ReleaseObj();
		parser->ReleaseObj();
		if(GetMaximumProcessorCount()!=PROCESSOR_LIMIT_INFINITE)
		{
			while(m_processorList.Count()>=GetMaximumProcessorCount())
			{
				m_processorList.WaitForListSizeDecrease();
			}
		}
	}	

	killConnection();

}

