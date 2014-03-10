#pragma once
#include "jsasiopacketbase.h"
class CJsAsioPacketFile :
	public CJsAsioPacketBase
{
public:
#pragma pack(1)
	typedef struct 
	{
		int			_FileBufSize;
		int			_FileType;
		int			_FileSize;
		char		_FileName[60];
	}_CJsAsio_PacketFile_;
#pragma pack()

	enum ENUM_PACKETFILE_TYPE
	{
		_PACKETFILE_TYPE_REQUEST	= 0,	//요청
		_PACKETFILE_TYPE_RESPONSE	= 1,	//응답
	};

protected:
	char*  buffer;

public:

	int			GetFileBufSize();
	void		SetFileBufSize(int nsize);
	int			GetFileType();
	void		SetFileType(int type);
	int			GetFileSize();
	void		SetFileSize(int nSize);
	std::string GetFileName();
	void		SetFileName(std::string strfile);

	bool		FileLoad(std::string strFile);
	bool		FileSave(std::string strFile);

public:
	virtual std::string GetTypeString();
	virtual bool IsBroadCast();

public:
	virtual size_t GetPacketSize();
	virtual char* GetPacketData();
	virtual CJsAsioPacketBase* ReadParse();
	virtual std::vector<CJsAsioPacketBase*> WriteParse();

public:
	CJsAsioPacketFile(void);
	CJsAsioPacketFile(int nBufSize);
	virtual ~CJsAsioPacketFile(void);

};

