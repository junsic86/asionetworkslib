#pragma once
#include <vector>
class CJsAsioPacketBase
{
public:
	enum CLASSTYPE
	{
		CLASSTYPE_AsioPacketBase		= 0,
		CLASSTYPE_AsioPacketStart		= 1,
		CLASSTYPE_AsioPacketHead		= 1,
		CLASSTYPE_AsioPacketTime		= 2,
		CLASSTYPE_AsioPacketFile		= 3,
		CLASSTYPE_AsioPacketEnd			= 3
	};

public:
	virtual size_t	GetPacketSize() = 0;
	virtual char*	GetPacketData() = 0;
	virtual CJsAsioPacketBase* ReadParse() = 0;
	virtual std::vector<CJsAsioPacketBase*> WriteParse() = 0;

public:
	virtual std::string GetTypeString() { return "PacketBase"; };
	virtual bool CheckType() { return true; }
	virtual bool IsBroadCast() { return false; };

public:
	CJsAsioPacketBase(void);
	virtual ~CJsAsioPacketBase(void);
};

