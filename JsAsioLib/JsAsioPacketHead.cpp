#include "StdAfx.h"

#include "JsAsioPacketHead.h"
#include "JsAsioPacketTime.h"
#include "JsAsioPacketFile.h"

CJsAsioPacketHead::CJsAsioPacketHead(void)
{
	SetHead('J', 'S', 'N', 'L');
	SetVersion(20140221);
}

CJsAsioPacketHead::~CJsAsioPacketHead(void)
{
}

char* CJsAsioPacketHead::GetPacketData()
{
	return buffer;
}

size_t CJsAsioPacketHead::GetPacketSize()
{
	return sizeof(buffer) * sizeof(char);
}

CJsAsioPacketBase* CJsAsioPacketHead::ReadParse()
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;

	if(packet->_type == CJsAsioPacketBase::CLASSTYPE_AsioPacketTime)
	{
		return new CJsAsioPacketTime();
	}
	else if(packet->_type == CJsAsioPacketBase::CLASSTYPE_AsioPacketFile)
	{
		return new CJsAsioPacketFile(packet->_BoadySize); 
	}

	return new CJsAsioPacketHead();
}

std::vector<CJsAsioPacketBase*> CJsAsioPacketHead::WriteParse()
{
	std::vector<CJsAsioPacketBase*> temp;
	return temp;
}

void CJsAsioPacketHead::SetHead( char h1, char h2, char h3, char h4 )
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;

	packet->_Head[0] = h1;
	packet->_Head[1] = h2;
	packet->_Head[2] = h3;
	packet->_Head[3] = h4;
}

void CJsAsioPacketHead::SetVersion( int Version )
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;
	packet->_version = Version;
}

void CJsAsioPacketHead::SetType( int Type )
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;
	packet->_type = Type;
}

void CJsAsioPacketHead::SetBoadySize( size_t Bodysize )
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;
	packet->_BoadySize = Bodysize;
}

bool CJsAsioPacketHead::CheckType()
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;

	if( packet->_Head[0] == 'J' &&
		packet->_Head[1] == 'S' &&
		packet->_Head[2] == 'N' &&
		packet->_Head[3] == 'L' )
	{
		return true;
	}
	else
	{
		return false;

	}
}

std::string CJsAsioPacketHead::GetTypeString()
{
	return "PacketHead";
}

bool CJsAsioPacketHead::IsBroadCast()
{
	return false;
}

size_t CJsAsioPacketHead::GetBoadySize()
{
	_CJsAsio_PacketHead_* packet = (_CJsAsio_PacketHead_*)buffer;
	return packet->_BoadySize;
}
