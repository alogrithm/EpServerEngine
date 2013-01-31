/*! 
BaseClientUDP for the EpServerEngine
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
#include "epBaseClientUDP.h"


using namespace epse;

BaseClientUDP::BaseClientUDP(const TCHAR * hostName, const TCHAR * port,SyncPolicy syncPolicy,unsigned int waitTimeMilliSec,epl::LockPolicy lockPolicyType): BaseServerSendObject(waitTimeMilliSec,lockPolicyType)
{
	m_lockPolicy=lockPolicyType;
	switch(lockPolicyType)
	{
	case epl::LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW epl::CriticalSectionEx();
		m_generalLock=EP_NEW epl::CriticalSectionEx();
		m_disconnectLock=EP_NEW epl::CriticalSectionEx();
		break;
	case epl::LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW epl::Mutex();
		m_generalLock=EP_NEW epl::Mutex();
		m_disconnectLock=EP_NEW epl::Mutex();
		break;
	case epl::LOCK_POLICY_NONE:
		m_sendLock=EP_NEW epl::NoLock();
		m_generalLock=EP_NEW epl::NoLock();
		m_disconnectLock=EP_NEW epl::NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		m_disconnectLock=NULL;
		break;
	}
	SetHostName(hostName);
	SetPort(port);
	m_connectSocket=INVALID_SOCKET;
	m_result=0;
	m_ptr=0;
	m_maxPacketSize=0;
	setSyncPolicy(syncPolicy);
	m_parserList=ParserList(syncPolicy,waitTimeMilliSec,lockPolicyType);
}

BaseClientUDP::BaseClientUDP(const BaseClientUDP& b):BaseServerSendObject(b)
{
	m_connectSocket=INVALID_SOCKET;
	m_result=0;
	m_ptr=0;
	m_hostName=b.m_hostName;
	m_port=b.m_port;
	m_lockPolicy=b.m_lockPolicy;
	m_maxPacketSize=b.m_maxPacketSize;
	switch(m_lockPolicy)
	{
	case epl::LOCK_POLICY_CRITICALSECTION:
		m_sendLock=EP_NEW epl::CriticalSectionEx();
		m_generalLock=EP_NEW epl::CriticalSectionEx();
		m_disconnectLock=EP_NEW epl::CriticalSectionEx();
		break;
	case epl::LOCK_POLICY_MUTEX:
		m_sendLock=EP_NEW epl::Mutex();
		m_generalLock=EP_NEW epl::Mutex();
		m_disconnectLock=EP_NEW epl::Mutex();
		break;
	case epl::LOCK_POLICY_NONE:
		m_sendLock=EP_NEW epl::NoLock();
		m_generalLock=EP_NEW epl::NoLock();
		m_disconnectLock=EP_NEW epl::NoLock();
		break;
	default:
		m_sendLock=NULL;
		m_generalLock=NULL;
		m_disconnectLock=NULL;
		break;
	}
	m_parserList=ParserList(m_syncPolicy,m_waitTime,m_lockPolicy);

}
BaseClientUDP::~BaseClientUDP()
{
	Disconnect();

	if(m_sendLock)
		EP_DELETE m_sendLock;
	if(m_generalLock)
		EP_DELETE m_generalLock;
	if(m_disconnectLock)
		EP_DELETE m_disconnectLock;
}

void BaseClientUDP::SetHostName(const TCHAR * hostName)
{
	epl::LockObj lock(m_generalLock);

	unsigned int strLength=epl::System::TcsLen(hostName);
	if(strLength==0)
		m_hostName=DEFAULT_HOSTNAME;
	else
	{		
#if defined(_UNICODE) || defined(UNICODE)
		m_hostName=epl::System::WideCharToMultiByte(hostName);
#else// defined(_UNICODE) || defined(UNICODE)
		m_hostName=hostName;
#endif// defined(_UNICODE) || defined(UNICODE)
	}
}

void BaseClientUDP::SetPort(const TCHAR *port)
{
	epl::LockObj lock(m_generalLock);

	unsigned int strLength=epl::System::TcsLen(port);
	if(strLength==0)
		m_port=DEFAULT_PORT;
	else
	{
#if defined(_UNICODE) || defined(UNICODE)
		m_port=epl::System::WideCharToMultiByte(port);
#else// defined(_UNICODE) || defined(UNICODE)
		m_port=port;
#endif// defined(_UNICODE) || defined(UNICODE)
	}

}
epl::EpTString BaseClientUDP::GetHostName() const
{
	epl::LockObj lock(m_generalLock);
	if(!m_hostName.length())
		return _T("");

#if defined(_UNICODE) || defined(UNICODE)
	epl::EpTString retString=epl::System::MultiByteToWideChar(m_hostName.c_str());
	return retString;
#else //defined(_UNICODE) || defined(UNICODE)
	return m_hostName;
#endif //defined(_UNICODE) || defined(UNICODE)

}
epl::EpTString BaseClientUDP::GetPort() const
{
	epl::LockObj lock(m_generalLock);
	if(!m_port.length())
		return _T("");

#if defined(_UNICODE) || defined(UNICODE)
	epl::EpTString retString=epl::System::MultiByteToWideChar(m_port.c_str());;
	return retString;
#else //defined(_UNICODE) || defined(UNICODE)
	return m_port;
#endif //defined(_UNICODE) || defined(UNICODE)

}

unsigned int BaseClientUDP::GetMaxPacketByteSize() const
{
	return m_maxPacketSize;
}

int BaseClientUDP::Send(const Packet &packet)
{
	epl::LockObj lock(m_sendLock);
	if(!IsConnected())
		return 0;
	int sentLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	EP_ASSERT(length<=m_maxPacketSize);
	if(length>0)
	{
		//int sentLength=send(m_connectSocket,packetData,length,0);
		sentLength=sendto(m_connectSocket,packetData,length,0,m_ptr->ai_addr,sizeof(sockaddr));
		if(sentLength<=0)
		{
			return sentLength;
		}
	}
	return sentLength;
}

int BaseClientUDP::receive(Packet &packet)
{

	int length=packet.GetPacketByteSize();
	char *packetData=const_cast<char*>(packet.GetPacket());
	sockaddr tmpInfo;
	int tmpInfoSize=sizeof(sockaddr);
	int recvLength = recvfrom(m_connectSocket,packetData,length,0,&tmpInfo,&tmpInfoSize);
	return recvLength;
}


vector<BaseServerObject*> BaseClientUDP::GetPacketParserList() const
{
	return m_parserList.GetList();
}


bool BaseClientUDP::Connect()
{
	epl::LockObj lock(m_generalLock);
	if(IsConnected())
		return true;
	if(m_syncPolicy!=SYNC_POLICY_ASYNCHRONOUS)
	{
		m_parserList.Clear();
		m_parserList.StartParse();
	}
	if(!m_port.length())
	{
		m_port=DEFAULT_PORT;
	}

	if(!m_hostName.length())
	{
		m_hostName=DEFAULT_HOSTNAME;
	}


	WSADATA wsaData;
	m_connectSocket = INVALID_SOCKET;
	m_maxPacketSize=0;
	struct addrinfo hints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		epl::System::OutputDebugString(_T("%s::%s(%d)(%x) WSAStartup failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		return false;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	// Resolve the server address and port
	iResult = getaddrinfo(m_hostName.c_str(), m_port.c_str(), &hints, &m_result);
	if ( iResult != 0 ) {
		epl::System::OutputDebugString(_T("%s::%s(%d)(%x) getaddrinfo failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for(m_ptr=m_result; m_ptr != NULL ;m_ptr=m_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		m_connectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, 
			m_ptr->ai_protocol);
		if (m_connectSocket == INVALID_SOCKET) {
			epl::System::OutputDebugString(_T("%s::%s(%d)(%x) Socket failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
			cleanUpClient();
			return false;
		}
		break;
	}
	if (m_connectSocket == INVALID_SOCKET) {
		epl::System::OutputDebugString(_T("%s::%s(%d)(%x) Unable to connect to server!\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
		cleanUpClient();
		return false;
	}

	int nTmp = sizeof(int);
	getsockopt(m_connectSocket, SOL_SOCKET,SO_MAX_MSG_SIZE, (char *)&m_maxPacketSize,&nTmp);

	if(Start())
	{
		return true;
	}
	cleanUpClient();
	return false;
}


bool BaseClientUDP::IsConnected() const
{
	return (GetStatus()==Thread::THREAD_STATUS_STARTED);
}

void BaseClientUDP::cleanUpClient()
{
	if(m_result)
	{
		freeaddrinfo(m_result);
		m_result=NULL;
	}
	if(m_connectSocket!=INVALID_SOCKET)
	{
		closesocket(m_connectSocket);
		m_connectSocket = INVALID_SOCKET;
	}
	m_maxPacketSize=0;
	WSACleanup();

}

void BaseClientUDP::disconnect(bool fromInternal)
{
	if(!m_disconnectLock->TryLock())
	{
		return;
	}


	if(IsConnected())
	{
		if(m_connectSocket!=INVALID_SOCKET)
		{
			int iResult = shutdown(m_connectSocket, SD_SEND);
			if (iResult == SOCKET_ERROR)
				epl::System::OutputDebugString(_T("%s::%s(%d)(%x) shutdown failed with error: %d\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this, WSAGetLastError());
			closesocket(m_connectSocket);
			m_connectSocket = INVALID_SOCKET;
		}
		
		if(!fromInternal)
			TerminateAfter(m_waitTime);
	
		m_parserList.Clear();
	}
	cleanUpClient();
	if(m_syncPolicy==SYNC_POLICY_SYNCHRONOUS)
	{
		m_parserList.StopParser();
	}

	m_disconnectLock->Unlock();
}

void BaseClientUDP::Disconnect()
{
	epl::LockObj lock(m_generalLock);
	if(!IsConnected())
	{
		return;
	}
	disconnect(false);
}


void BaseClientUDP::execute() 
{
	int iResult=0;
	// Receive until the peer shuts down the connection
	Packet recvPacket(NULL,m_maxPacketSize);
	do {
		iResult = receive(recvPacket);
		
		if (iResult > 0) {
			BasePacketParser::PacketPassUnit passUnit;
			Packet *passPacket=EP_NEW Packet(recvPacket.GetPacket(),iResult);
			passUnit.m_packet=passPacket;
			passUnit.m_this=this;
			BasePacketParser *parser=createNewPacketParser();
			parser->setSyncPolicy(m_syncPolicy);
			if(m_syncPolicy==SYNC_POLICY_ASYNCHRONOUS)
				parser->Start(reinterpret_cast<void*>(&passUnit));
			else
				parser->setPacketPassUnit(&passUnit);
			m_parserList.Push(parser);
			parser->ReleaseObj();
			passPacket->ReleaseObj();
		}
		else if (iResult == 0)
		{
			epl::System::OutputDebugString(_T("%s::%s(%d)(%x) Connection closing...\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
			break;
		}
		else  {
			epl::System::OutputDebugString(_T("%s::%s(%d)(%x) recv failed with error\r\n"),__TFILE__,__TFUNCTION__,__LINE__,this);
			break;
		}
		m_parserList.RemoveTerminated();

	} while (iResult > 0);
	disconnect(true);
}