#pragma once

#include "JsAsioBoost_h.h"

class CJsAsioSessionGroup;
class CJsAsioSession;

class CJsAsioAccept
{
private:
	boost::asio::ip::tcp::acceptor				_acceptor;
	boost::shared_ptr<CJsAsioSessionGroup>		_session;

private:
	void StartAccept();
	void handle_accept(boost::shared_ptr<CJsAsioSession> new_connect,
		const boost::system::error_code& error);

public:
	CJsAsioAccept(boost::asio::io_service& io_service, boost::shared_ptr<CJsAsioSessionGroup> session, UINT16 port);
	~CJsAsioAccept(void);
};

