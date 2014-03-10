#pragma once

#include "JsAsioBoost_h.h"

class CJsAsioSession;

class CJsAsioSessionBase
{
protected:
	boost::weak_ptr<CJsAsioSession> _Session;

public:
	boost::weak_ptr<CJsAsioSession> GetSession();

public:
	CJsAsioSessionBase(boost::weak_ptr<CJsAsioSession> Session);
	~CJsAsioSessionBase(void);
};

