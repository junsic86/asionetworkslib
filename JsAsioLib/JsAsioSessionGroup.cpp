#include "StdAfx.h"
#include "JsAsioSessionGroup.h"

#include "JsAsioSession.h"
#include "JsAsioAccept.h"
#include "JsAsioThreadPool.h"
#include "JsAsioServer.h"
#include "JsAsioSessionBase.h"

CJsAsioSessionGroup::CJsAsioSessionGroup(void)
{
}


CJsAsioSessionGroup::~CJsAsioSessionGroup(void)
{
}

void CJsAsioSessionGroup::SessionGroupRegster( boost::weak_ptr<CJsAsioSessionBase> Connect )
{
	boost::lock_guard<boost::mutex> lock(_mutex);
	SessionGroup.insert(Connect);
	LOGI("技记 殿废"<<SessionGroup.size());
	//std::cout<<"SessionRegster >>"<<Session.size()<<std::endl;
}

void CJsAsioSessionGroup::SessionGroupRelease( boost::weak_ptr<CJsAsioSessionBase> Connect )
{
	boost::lock_guard<boost::mutex> lock(_mutex);
	SessionGroup.erase(Connect);
	LOGI("技记 秦力"<<SessionGroup.size());
	//std::cout<<"SessionRelease error >>"<<Session.size()<<std::endl;
}

void CJsAsioSessionGroup::GetAllPacketFactory( std::vector <boost::shared_ptr<CJsAsioPacketFactory> >& vpPacketFactory )
{
	boost::lock_guard<boost::mutex> lock(_mutex);
	BOOST_FOREACH(boost::weak_ptr<CJsAsioSessionBase> sessionbase, SessionGroup)
	{
		boost::shared_ptr<CJsAsioSessionBase>	SessionBase(sessionbase.lock());
		if(!SessionBase) continue;

		boost::shared_ptr<CJsAsioSession>		Session(SessionBase->GetSession().lock());
		if(!Session) continue;

		boost::shared_ptr<CJsAsioPacketFactory>	Factory(Session->_packetFactory.lock());
		if(!Factory) continue;

		vpPacketFactory.push_back(Factory);
	}

	return;
}
