#pragma once
#include "JsAsioPacketBase.h"

class CJsAsioPacketHead : public CJsAsioPacketBase
{
public:
#pragma pack(1)
	typedef struct 
	{
		char		_Head[4];
		int			_version;
		int			_type;
		int			_BoadySize;
	}_CJsAsio_PacketHead_;
#pragma pack()

public:
	char buffer[100];

public:
	virtual size_t	GetPacketSize();;
	virtual char*	GetPacketData();;
	virtual CJsAsioPacketBase* ReadParse();
	virtual std::vector<CJsAsioPacketBase*> WriteParse();
	virtual bool CheckType();

public:
	void SetHead(char h1, char h2, char h3, char h4);
	void SetVersion(int Version);
	void SetType(int Type);
	void SetBoadySize(size_t Bodysize);
	size_t GetBoadySize();

public:
	CJsAsioPacketHead(void);
	virtual ~CJsAsioPacketHead(void);

	virtual std::string GetTypeString();

	virtual bool IsBroadCast();

};

