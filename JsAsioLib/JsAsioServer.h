#pragma once

#include "JsAsioBoost_h.h"

class CJsAsioThreadPool;
class CJsAsioSessionGroup;
class CJsAsioAccept;

class CJsAsioServer
{
protected:
	boost::asio::io_service							_io_service;
	boost::shared_ptr<CJsAsioThreadPool>			_threadpool;
	boost::shared_ptr<CJsAsioSessionGroup>			_session;
	boost::shared_ptr<CJsAsioAccept>				_accept;

protected:
	UINT16											_Port;
	bool											_bStart;

public:
	bool	IsStartServer();
	UINT16	GetPort();
	void	SetPort(UINT16 Port);

	void	ServerStart();
	void	ServerStop();

public:
	CJsAsioServer(void);
	~CJsAsioServer(void);
};

