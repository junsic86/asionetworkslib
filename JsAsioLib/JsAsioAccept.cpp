#include "stdafx.h"
#include "JsAsioAccept.h"

#include "JsAsioSessionGroup.h"
#include "JsAsioSession.h"
#include "JsAsioThreadPool.h"
#include "JsAsioServer.h"

CJsAsioAccept::CJsAsioAccept( boost::asio::io_service& io_service, boost::shared_ptr<CJsAsioSessionGroup> session, UINT16 port )
	: _session(session),
	_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	StartAccept();
}

CJsAsioAccept::~CJsAsioAccept(void)
{
}

void CJsAsioAccept::StartAccept()
{
	CJsAsioSession::SharedPtrConnect new_connect =
		CJsAsioSession::CreateConnect( _acceptor.get_io_service() );

	_acceptor.async_accept(new_connect->GetSocket(),
		boost::bind(&CJsAsioAccept::handle_accept, this, new_connect,
		boost::asio::placeholders::error));

}

void CJsAsioAccept::handle_accept( boost::shared_ptr<CJsAsioSession> new_connect, const boost::system::error_code& error )
{
	if (!error)
	{
		new_connect->StartSession( _session );
	}
	else
	{
		LOGE(error.message());
		//std::cout<<"error accept >>"<<error.message()<<std::endl;
		return;
	}

	StartAccept();
}
