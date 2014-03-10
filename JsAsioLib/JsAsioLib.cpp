// JsAsioLib.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <time.h>
#include <conio.h>
#include <boost\type_traits.hpp>
#include "JsAsioServer.h"

boost::mutex g_mutex;

void process();

int _tmain(int argc, _TCHAR* argv[])
{

	process();
	return 0;
}



template <typename T1>
void aaa(T1 aa)
{
	if(::boost::is_float<T1>::value)
	{
		std::cout<<"aa";
	}
	else
	{
		std::cout<<"bb";
	}
}

void process()
{
	CJsAsioServer Server;
	Server.ServerStart();
	while(1)
	{
		if( _kbhit() )
			break;
		Sleep(10);
	}

}


