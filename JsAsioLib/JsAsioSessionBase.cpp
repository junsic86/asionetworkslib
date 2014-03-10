#include "StdAfx.h"
#include "JsAsioSessionBase.h"


CJsAsioSessionBase::CJsAsioSessionBase( boost::weak_ptr<CJsAsioSession> Session )
	: _Session(Session)
{

}

CJsAsioSessionBase::~CJsAsioSessionBase(void)
{
}

boost::weak_ptr<CJsAsioSession> CJsAsioSessionBase::GetSession()
{
	return _Session;
}
