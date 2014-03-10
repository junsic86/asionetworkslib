#include "StdAfx.h"
#include "JsAsioPacketFactory.h"

#include "JsAsioSession.h"
#include "JsAsioPacketBase.h"
#include "JsAsioPacketHead.h"

CJsAsioPacketFactory::CJsAsioPacketFactory( boost::shared_ptr<CJsAsioSession> session )
	: _session( boost::shared_ptr<CJsAsioSession>(session) ),
	_strand(session->GetSocket().get_io_service())
{
	_packet_SendQue.SetEndQue(TRUE);
}

CJsAsioPacketFactory::~CJsAsioPacketFactory(void)
{

}

void CJsAsioPacketFactory::handle_write( const boost::system::error_code& error, size_t bytes_transferred )
{
	if (!error)
	{
		LOGI("Send :"<<"size = "<<bytes_transferred<<" Port = "<<_session->GetSocket().remote_endpoint().port());

		_packet_SendQue.pop_front();
		if(!_packet_SendQue.empty())
		{
			boost::asio::async_write( _session->GetSocket(), boost::asio::buffer( _packet_SendQue.front()->GetPacketData(), _packet_SendQue.front()->GetPacketSize() ),
				_strand.wrap( boost::bind(&CJsAsioPacketFactory::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred) ) );
		}
		else
		{
			boost::lock_guard<boost::mutex> lock( _mutex ); 
			_packet_SendQue.SetEndQue(TRUE);
		}
	}
	else
	{
		handle_error(error, bytes_transferred);
	}
}

void CJsAsioPacketFactory::handle_read( const boost::system::error_code& error, size_t bytes_transferred )
{

	if (!error)
	{
		LOGI("Recv :"<<"size = "<<bytes_transferred<<" Port = "<<_session->GetSocket().remote_endpoint().port()<<" Packet : "<< _packet->GetTypeString() );

		if(!_packet->CheckType())
		{
			LOGE("헤더 정보 불일치 연결 종료"<<"Port : "<<_session->GetSocket().remote_endpoint().port());
			_session->GetSocket().close();
			return;
		}

		PacketSendPushQue( _packet->WriteParse() );
		_packet.reset( _packet->ReadParse() );

		boost::asio::async_read(_session->GetSocket(), boost::asio::buffer( _packet->GetPacketData(), _packet->GetPacketSize() ),
			_strand.wrap( boost::bind(&CJsAsioPacketFactory::handle_read, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred) ) );
	}
	else
	{
		handle_error(error, bytes_transferred);
	}

}

void CJsAsioPacketFactory::handle_error( const boost::system::error_code& error, size_t bytes_transferred )
{
	if(error == boost::asio::error::eof)
		{
			LOGW("연결 종료"<<"Port : "<<_session->GetSocket().remote_endpoint().port());
			//std::cout<<"연결이 정상적으로 종료"<<std::endl;
		}
		else
		{
			LOGE(error.message());
			/*
			std::cout << "error No: " << error.value() << " error Message: "
				<< error.message() << std::endl;
			*/
		}
}

void CJsAsioPacketFactory::PacketRecvStart()
{
	_packet.reset( dynamic_cast<CJsAsioPacketBase*>( new CJsAsioPacketHead() ) );

	boost::asio::async_read(_session->GetSocket(), boost::asio::buffer( _packet->GetPacketData(), _packet->GetPacketSize() ),
		_strand.wrap( boost::bind(&CJsAsioPacketFactory::handle_read, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred) ) );
}

void CJsAsioPacketFactory::PacketSendStart()
{
	boost::lock_guard<boost::mutex> lock( _mutex ); 

	if( !_packet_SendQue.empty() && _packet_SendQue.IsEndQue() )
	{
		_packet_SendQue.SetEndQue(FALSE);

		boost::asio::async_write(_session->GetSocket(), boost::asio::buffer( _packet_SendQue.front()->GetPacketData(), _packet_SendQue.front()->GetPacketSize() ),
			_strand.wrap( boost::bind(&CJsAsioPacketFactory::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred) ) );
	}
}

void CJsAsioPacketFactory::PacketSendPushQue( std::vector<CJsAsioPacketBase*> vpSendPacket )
{
	if( !vpSendPacket.empty() )
	{
		std::vector<boost::shared_ptr< CJsAsioPacketBase >> vpPushQue;

		BOOST_FOREACH(CJsAsioPacketBase* SendPacket, vpSendPacket)
		{
			vpPushQue.push_back( boost::shared_ptr< CJsAsioPacketBase >(SendPacket) );
		}

		if(true == vpSendPacket[0]->IsBroadCast() == true )
		{
			std::vector<boost::shared_ptr<CJsAsioPacketFactory> >  vpBroadFactory;
			_session->_sessionGroup->GetAllPacketFactory( vpBroadFactory );

			BOOST_FOREACH(boost::shared_ptr<CJsAsioPacketFactory> BroadFactory, vpBroadFactory)
			{
				if(BroadFactory && shared_from_this() != BroadFactory)
				{
					BroadFactory->_packet_SendQue.vector_push_back( vpPushQue );
					BroadFactory->PacketSendStart();
				}
			}
		}
		else
		{
			_packet_SendQue.vector_push_back( vpPushQue );
			PacketSendStart();
		}
	}	
}

