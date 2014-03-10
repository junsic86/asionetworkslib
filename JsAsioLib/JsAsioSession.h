#pragma once

#include "JsAsioBoost_h.h"
#include "JsAsioSessionGroup.h"

class CJsAsioSessionGroup;
class CJsAsioSessionBase;
class CJsAsioPacketFactory;

class CJsAsioSession
	: public boost::enable_shared_from_this<CJsAsioSession>
{

public:
	typedef boost::shared_ptr<CJsAsioSession> SharedPtrConnect;

	static SharedPtrConnect CreateConnect(boost::asio::io_service& io_service)
	{
		return SharedPtrConnect( new CJsAsioSession(io_service) );
	}

public:
	boost::asio::ip::tcp::socket _socket;
	boost::asio::ip::tcp::socket& GetSocket();

public:
	boost::shared_ptr<CJsAsioSessionGroup>	_sessionGroup;
	boost::shared_ptr<CJsAsioSessionBase>	_sessionBase;
	boost::weak_ptr<CJsAsioPacketFactory>	_packetFactory;

public:
	void StartSession( boost::shared_ptr<CJsAsioSessionGroup> session );

public:
	CJsAsioSession(boost::asio::io_service& io_service);
	~CJsAsioSession(void);
};

