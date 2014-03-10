// JsAsioClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"



#include "../JsAsioLib/JsAsioPacketBase.h"
#include "../JsAsioLib/JsAsioPacketHead.h"
#include "../JsAsioLib/JsAsioPacketTime.h"
#include "../JsAsioLib/JsAsioPacketFile.h"


std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);

	char strTIme[300];
	ctime_s(strTIme, sizeof(strTIme) / sizeof(char), &now);

	return strTIme;
}

using boost::asio::ip::tcp;

//void client()
//{
//	try
//	{
//		std::cout << "client" << "\n";
//
//		boost::asio::io_service io;
//
//		tcp::resolver resolver(io);
//		tcp::resolver::query query("127.0.0.1", "daytime");
//
//		tcp::socket socket(io);
//		boost::system::error_code error = boost::asio::error::host_not_found;
//
//
//		tcp::endpoint endpoint;
//		endpoint.address(boost::asio::ip::address::from_string("192.168.2.134"));
//		endpoint.port(20201);
//
//
//		socket.close();
//		socket.connect(endpoint, error);
//
//		if (error)
//			throw boost::system::system_error(error);
//
//		for ( int i = 0; i < 1000; i++ )
//		{
//			boost::system::error_code error;
//
//			CJsAsioPacketHead _haed;
//			CJsAsioPacketTime _time;
//
//			_haed.SetType(CJsAsioPacketBase::CLASSTYPE_AsioPacketTime);
//			_haed.SetBoadySize( (int)_time.GetPacketSize() ); 
//
//			strcpy_s( _time.GetPacketData(), _time.GetPacketSize(), make_daytime_string().c_str() );
//
//			boost::asio::write(socket, boost::asio::buffer(_haed.GetPacketData(), _haed.GetPacketSize()));
//			boost::asio::write(socket, boost::asio::buffer(_time.GetPacketData(), _time.GetPacketSize()));
//
//
//			//boost::asio::read(socket, boost::asio::buffer(_haed.GetPacketData(), _haed.GetPacketSize()));
//			//boost::asio::read(socket, boost::asio::buffer(_time.GetPacketData(), _time.GetPacketSize()));
//
//			std::cout<<"SEND 성공"<<_time.GetPacketData()<<std::endl;
//
//			Sleep(1000);
//			//_sleep(10);
//
//			//size_t len = boost::asio::read(socket, boost::asio::buffer(buf), boost::asio::transfer_all(), error);
//			/*
//			std::string message_ = make_daytime_string();
//
//			int aa = boost::asio::write(socket, boost::asio::buffer(message_));
//			std::cout<<"전송 : "<<aa<<std::endl;
//
//			aa = boost::asio::write(socket, boost::asio::buffer(message_));
//			std::cout<<"전송 : "<<aa<<std::endl;
//
//			aa = boost::asio::write(socket, boost::asio::buffer(message_));
//			std::cout<<"전송 : "<<aa<<std::endl;
//
//			aa = boost::asio::write(socket, boost::asio::buffer(message_));
//			std::cout<<"전송 : "<<aa<<std::endl;
//			
//			_sleep(300);
//			*/
//			
//
//			//socket.shutdown(tcp::socket::shutdown_receive, error);
//			//break;
//
//			//socket.close();
//			//break;
//			//size_t len = socket.receive(boost::asio::buffer(buf), 0,  error);
//
//			/*
//			if (error == boost::asio::error::eof)
//				break; // Connection closed cleanly by peer.
//			else if (error)
//				throw boost::system::system_error(error); // Some other error.
//
//			*/
//			//std::cout.write(buf.data(), len);
//		}
//
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//	}
//}

void client4()
{
	try
	{
		std::cout << "client" << "\n";

		boost::asio::io_service io;

		tcp::resolver resolver(io);
		tcp::resolver::query query("127.0.0.1", "daytime");

		tcp::socket socket(io);
		boost::system::error_code error = boost::asio::error::host_not_found;


		tcp::endpoint endpoint;
		endpoint.address(boost::asio::ip::address::from_string("192.168.2.134"));
		endpoint.port(20201);


		socket.close();
		socket.connect(endpoint, error);

		if (error)
			throw boost::system::system_error(error);

		for ( int i = 0; i < 1000; i++ )
		{
			boost::system::error_code error;

			CJsAsioPacketHead _haed;
			CJsAsioPacketFile _file;

			_file.SetFileType(CJsAsioPacketFile::_PACKETFILE_TYPE_REQUEST);

			_haed.SetType(CJsAsioPacketBase::CLASSTYPE_AsioPacketFile);
			_haed.SetBoadySize( (int)_file.GetPacketSize() ); 

			boost::asio::write(socket, boost::asio::buffer(_haed.GetPacketData(), _haed.GetPacketSize()));
			boost::asio::write(socket, boost::asio::buffer(_file.GetPacketData(), _file.GetPacketSize()));
	
			std::cout<<"요청완료 : "<<std::endl;

			boost::asio::read(socket, boost::asio::buffer(_haed.GetPacketData(), _haed.GetPacketSize()));
			CJsAsioPacketBase* _pfile = _haed.ReadParse();

			std::cout<<_haed.GetBoadySize()<<std::endl;


			boost::asio::read(socket, boost::asio::buffer(_pfile->GetPacketData(), _pfile->GetPacketSize()));

			std::cout<<"전송완료 : "<<std::endl;

			((CJsAsioPacketFile*)_pfile)->FileSave("d:\\barcodeextra2.PNG");
		
			Sleep(1000);
			break;
			
			//std::cout.write(buf.data(), len);
		}

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{

	while(1)
	{
		
		boost::thread t1(client4);
		/*boost::thread t2(client2);
		boost::thread t3(client2);
		boost::thread t4(client2);*/

		t1.join();

		break;
		/*t2.join();
		t3.join();
		t4.join();*/
		
		if( _kbhit() )
			break;
		Sleep(1000);
	}

	return 0;
}

