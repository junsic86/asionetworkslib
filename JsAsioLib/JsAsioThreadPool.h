#pragma once
#include "JsAsioBoost_h.h"

class CJsAsioThreadPool
{
public:
	boost::asio::io_service& _io_service;
	boost::shared_ptr<boost::asio::io_service::work> _work;
	boost::thread_group _thradGroup;

public:
	template <class F>
	void post(F f)
	{
		io_service_.post(f);
	}

public:
	void Init(boost::asio::io_service& io_service, std::size_t ThreadCreateSize);
	CJsAsioThreadPool(boost::asio::io_service& io_service, std::size_t ThreadCreateSize);
	CJsAsioThreadPool(boost::asio::io_service& io_service);
	~CJsAsioThreadPool(void);
};

