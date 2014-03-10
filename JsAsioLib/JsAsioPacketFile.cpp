#include "stdafx.h"
#include "JsAsioPacketFile.h"
#include "JsAsioPacketHead.h"

#include <fstream>
#include <iostream>

CJsAsioPacketFile::CJsAsioPacketFile(void)
{
	buffer = new char[sizeof(_CJsAsio_PacketFile_)];

	SetFileBufSize( sizeof(_CJsAsio_PacketFile_));
}

CJsAsioPacketFile::CJsAsioPacketFile( int nBufSize )
{
	buffer = new char[nBufSize];

	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	SetFileBufSize( nBufSize );
}

CJsAsioPacketFile::~CJsAsioPacketFile(void)
{
	if(buffer)
		delete[] buffer;
}

std::string CJsAsioPacketFile::GetTypeString()
{
	return "PacketFile";
}

bool CJsAsioPacketFile::IsBroadCast()
{
	return false;
}


size_t CJsAsioPacketFile::GetPacketSize()
{
	return GetFileBufSize();
}

char* CJsAsioPacketFile::GetPacketData()
{
	return buffer;
}


CJsAsioPacketBase* CJsAsioPacketFile::ReadParse()
{
	return new CJsAsioPacketHead();
}

std::vector<CJsAsioPacketBase*> CJsAsioPacketFile::WriteParse()
{
	std::vector<CJsAsioPacketBase*> vpPacket;
	
	//요청 패킷 체크
	if( _PACKETFILE_TYPE_REQUEST == GetFileType() )
	{
		CJsAsioPacketHead* _head = new CJsAsioPacketHead();
		CJsAsioPacketFile* _File = new CJsAsioPacketFile();

		_File->FileLoad("d:\\barcodeextra.PNG");
		_File->SetFileName( "SqlLib.zip" );
		_File->SetFileType( _PACKETFILE_TYPE_RESPONSE );
	
		_head->SetBoadySize(_File->GetPacketSize());
		_head->SetType(CJsAsioPacketBase::CLASSTYPE_AsioPacketFile);
		

		vpPacket.push_back(_head);
		vpPacket.push_back(_File);
	}

	return vpPacket;
}

bool CJsAsioPacketFile::FileLoad( std::string strFile )
{
	
	try
	{
		std::ifstream inFile(strFile, std::ios::in | std::ios::binary);
		if( !inFile.is_open() )
			return false;

		inFile.seekg(0, std::ios_base::end);
		int nSize = inFile.tellg();

		SetFileBufSize( nSize + sizeof(_CJsAsio_PacketFile_) );
		SetFileSize(nSize);
		_CJsAsio_PacketFile_ FileHead;

		if(buffer)
		{
			memcpy_s( &FileHead, sizeof(_CJsAsio_PacketFile_), buffer, sizeof(_CJsAsio_PacketFile_) );
			delete[] buffer;
		}
		buffer = new char[ FileHead._FileBufSize ];
		memcpy_s( buffer, sizeof(_CJsAsio_PacketFile_), &FileHead, sizeof(_CJsAsio_PacketFile_) );

		inFile.seekg(0, std::ios_base::beg);
		inFile.read(buffer + sizeof(_CJsAsio_PacketFile_), nSize );
		inFile.close();

	}
	catch(std::exception& e)
	{
		return false;
	}

	return true;
}

bool CJsAsioPacketFile::FileSave( std::string strFile )
{

	try
	{
		std::ofstream inFile(strFile, std::ios::out | std::ios::binary);
		if( !inFile.is_open() )
			return false;

		inFile.write(buffer + sizeof(_CJsAsio_PacketFile_), GetFileSize() );
		inFile.close();

	}
	catch(std::exception& e)
	{
		return false;
	}

	return true;
}

int CJsAsioPacketFile::GetFileBufSize()
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	return packet->_FileBufSize;
}

int CJsAsioPacketFile::GetFileType()
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	return packet->_FileType;
}

void CJsAsioPacketFile::SetFileType( int type )
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	packet->_FileType = type;
}

void CJsAsioPacketFile::SetFileSize( int nSize )
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	packet->_FileSize = nSize;
}

std::string CJsAsioPacketFile::GetFileName()
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	return std::string(packet->_FileName);
}

void CJsAsioPacketFile::SetFileName( std::string strfile )
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	strcpy_s(packet->_FileName, sizeof(packet->_FileName) / sizeof(char), strfile.c_str());
}

int CJsAsioPacketFile::GetFileSize()
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	return packet->_FileSize;
}

void CJsAsioPacketFile::SetFileBufSize( int nsize )
{
	_CJsAsio_PacketFile_* packet = (_CJsAsio_PacketFile_*)buffer;
	packet->_FileBufSize = nsize;
}
