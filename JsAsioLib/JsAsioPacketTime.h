#pragma once
#include "jsasiopacketbase.h"

class CJsAsioPacketTime : public CJsAsioPacketBase
{
public:
	char* buffer;

public:
	virtual size_t	GetPacketSize();;
	virtual char*	GetPacketData();;

	virtual CJsAsioPacketBase* ReadParse();
	virtual std::vector<CJsAsioPacketBase*> WriteParse();

public:
	CJsAsioPacketTime(void);
	virtual ~CJsAsioPacketTime(void);

	virtual std::string GetTypeString();

};

