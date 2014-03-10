#pragma once

#include "JsAsioBoost_h.h"
#include "JsAsioSession.h"
#include <set>
#include <vector>

class CJsAsioSession;
class CJsAsioSessionBase;
class CJsAsioPacketFactory;

class CJsAsioSessionGroup
	: public boost::enable_shared_from_this<CJsAsioSessionGroup>
{
private:
	std::set<boost::weak_ptr<CJsAsioSessionBase>> SessionGroup;
	boost::mutex _mutex;

public:
	void SessionGroupRegster(boost::weak_ptr<CJsAsioSessionBase> Connect);
	void SessionGroupRelease(boost::weak_ptr<CJsAsioSessionBase> Connect);
	void GetAllPacketFactory(std::vector <boost::shared_ptr<CJsAsioPacketFactory> >& vpFactory);

public:
	CJsAsioSessionGroup(void);
	~CJsAsioSessionGroup(void);
};

