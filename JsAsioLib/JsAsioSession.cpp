#include "stdafx.h"
#include "JsAsioSession.h"

#include "JsAsioSessionGroup.h"
#include "JsAsioAccept.h"
#include "JsAsioThreadPool.h"
#include "JsAsioServer.h"
#include "JsAsioSessionBase.h"
#include "JsAsioPacketFactory.h"

CJsAsioSession::~CJsAsioSession(void)
{
	if(_sessionBase.unique())
		_sessionGroup->SessionGroupRelease( _sessionBase );
}

CJsAsioSession::CJsAsioSession( boost::asio::io_service& io_service )
	: _socket(io_service)
{
	
}

boost::asio::ip::tcp::socket& CJsAsioSession::GetSocket()
{
	return _socket;
}

void CJsAsioSession::StartSession( boost::shared_ptr<CJsAsioSessionGroup> session )
{
	_sessionGroup = session;
	_sessionBase = boost::shared_ptr<CJsAsioSessionBase>(new CJsAsioSessionBase(shared_from_this()));
	_sessionGroup->SessionGroupRegster(_sessionBase);

	boost::shared_ptr<CJsAsioPacketFactory> Packet(new CJsAsioPacketFactory( shared_from_this() ));
	_packetFactory = Packet;
	Packet->PacketRecvStart();
}



