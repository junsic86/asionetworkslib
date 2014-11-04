#include "StdAfx.h"

#include "JsAsioPacketTime.h"
#include "JsAsioPacketHead.h"
#include <iostream>


CJsAsioPacketTime::CJsAsioPacketTime(void)
{
	buffer = new char[200];
}

CJsAsioPacketTime::~CJsAsioPacketTime(void)
{
	delete []buffer;
}

size_t CJsAsioPacketTime::GetPacketSize()
{
	return 200 * sizeof(char);
}

char* CJsAsioPacketTime::GetPacketData()
{
	return buffer;
}

CJsAsioPacketBase* CJsAsioPacketTime::ReadParse()
{
	//std::cout<<"time : " << buffer;
	return new CJsAsioPacketHead();
}

std::vector<CJsAsioPacketBase*> CJsAsioPacketTime::WriteParse()
{
	std::vector<CJsAsioPacketBase*> temp;

	//TEST ÆÐÅ¶
	CJsAsioPacketHead* _haed = new CJsAsioPacketHead();
	CJsAsioPacketTime* _time = new CJsAsioPacketTime();

	_haed->SetType(CJsAsioPacketBase::CLASSTYPE_AsioPacketTime);
	_haed->SetBoadySize( _time->GetPacketSize() ); 

	strcpy_s(_time->GetPacketData(), _time->GetPacketSize(),  "send 0000³â");

	temp.push_back(_haed);
	temp.push_back(_time);

	return temp;
}

std::string CJsAsioPacketTime::GetTypeString()
{
	return "PacketTime";
}
