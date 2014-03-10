#pragma once

#include "JsAsioBoost_h.h"
#include "SafeDeque.hpp"
#include <deque>
#include <vector>

class CJsAsioPacketBase;
class CJsAsioSession;

class CJsAsioPacketFactory
	: public boost::enable_shared_from_this<CJsAsioPacketFactory>
{
protected:
	boost::asio::strand									_strand;
	boost::shared_ptr<CJsAsioSession>					_session;
	boost::shared_ptr<CJsAsioPacketBase>				_packet;
	CSafeDeque<boost::shared_ptr< CJsAsioPacketBase >>	_packet_SendQue;
	boost::mutex										_mutex;		

public:
	CJsAsioPacketFactory(boost::shared_ptr<CJsAsioSession> session);
	virtual ~CJsAsioPacketFactory(void);

public:
	void PacketRecvStart();
	void PacketSendStart();
	void PacketSendPushQue( std::vector<CJsAsioPacketBase*> vpSendPacket );
	void handle_write(const boost::system::error_code& error,  size_t bytes_transferred);
	void handle_read(const boost::system::error_code& error,  size_t bytes_transferred);
	void handle_error(const boost::system::error_code& error,  size_t bytes_transferred);
};

