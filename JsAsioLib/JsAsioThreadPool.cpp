#include "StdAfx.h"
#include "JsAsioThreadPool.h"

#include "JsAsioSessionGroup.h"
#include "JsAsioSession.h"
#include "JsAsioAccept.h"
#include "JsAsioServer.h"

#pragma comment(lib, "user32.lib")

void CJsAsioThreadPool::Init( boost::asio::io_service& io_service, std::size_t ThreadCreateSize )
{
	_work.reset(new boost::asio::io_service::work(_io_service));

	for ( std::size_t i = 0; i < ThreadCreateSize; ++i ) 
	{
		_thradGroup.create_thread(boost::bind(&boost::asio::io_service::run, &_io_service));
	}

	LOGI("CJsAsioServer Thread Pool Start");
}

CJsAsioThreadPool::CJsAsioThreadPool( boost::asio::io_service& io_service )
	: _io_service(io_service)
{
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 

	Init(io_service, siSysInfo.dwNumberOfProcessors * 2);
}

CJsAsioThreadPool::CJsAsioThreadPool( boost::asio::io_service& io_service, std::size_t ThreadCreateSize )
	: _io_service(io_service)
{
	Init(io_service, ThreadCreateSize);
}

CJsAsioThreadPool::~CJsAsioThreadPool(void)
{
	_work.reset();
	_thradGroup.join_all();

	LOGI("CJsAsioServer Thread Pool End");
}

