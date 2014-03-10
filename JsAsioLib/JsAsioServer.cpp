#include "StdAfx.h"
#include "JsAsioServer.h"

#include "JsAsioSessionGroup.h"
#include "JsAsioSession.h"
#include "JsAsioAccept.h"
#include "JsAsioThreadPool.h"

CJsAsioServer::CJsAsioServer(void)
{
	_Port = 20201;
	_bStart = false;

	zsummer::log4z::ILog4zManager::GetInstance()->Start();
}

CJsAsioServer::~CJsAsioServer(void)
{
	
}

void CJsAsioServer::ServerStop()
{
	_bStart = false;
	_io_service.stop();

	LOGI("CJsAsioServer Stop");
}

void CJsAsioServer::ServerStart()
{
	if(_bStart)
		return;

	_bStart = true;
	_io_service.reset();

	_threadpool.reset( new CJsAsioThreadPool( _io_service ) );
	_session.reset( new CJsAsioSessionGroup() );
	_accept.reset( new CJsAsioAccept( _io_service, _session, _Port) );

	LOGI("CJsAsioServer Start");
}

bool CJsAsioServer::IsStartServer()
{
	return _bStart;
}

UINT16 CJsAsioServer::GetPort()
{
	return _Port;
}

void CJsAsioServer::SetPort(UINT16 Port)
{
	_Port = Port;
}
