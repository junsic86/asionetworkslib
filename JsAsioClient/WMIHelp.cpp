#include "stdafx.h"
#include "WMIHelp.h"

#include <iostream>
using namespace std;

# pragma comment(lib, "wbemuuid.lib")

int WMIHelp::InitCom()
{
	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. Error code = 0x" 
			<< hex << hres << endl;
		return 1;                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hres =  CoInitializeSecurity(
		NULL, 
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres))
	{
		cout << "Failed to initialize security. Error code = 0x" 
			<< hex << hres << endl;
		CoUninitialize();
		return 1;                    // Program has failed.
	}
}

int WMIHelp::InitWmi()
{
	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &pLoc);

	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1;                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x" 
			<< hex << hres << endl;
		pLoc->Release();     
		CoUninitialize();
		return 1;                // Program has failed.
	}

	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return 1;               // Program has failed.
	}

}

int WMIHelp::GetNetWorkTrafficPerSec(int &nByteSend, int&nByteRecv)
{
	nByteSend = 0;
	nByteRecv = 0;

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t("SELECT * FROM Win32_PerfFormattedData_Tcpip_NetworkInterface"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);

	/*
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t("SELECT * FROM Win32_PerfRawData_Tcpip_NetworkInterface"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);
	*/

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property
		hr = pclsObj->Get(L"BytesSentPersec", 0, &vtProp, 0, 0);
		//wcout << " BytesSentPersec : " << vtProp.bstrVal << endl;
		int tempSend = _wtoi(vtProp.bstrVal);

		hr = pclsObj->Get(L"BytesReceivedPersec", 0, &vtProp, 0, 0);
		//wcout << " BytesReceivedPersec : " << vtProp.bstrVal << endl;
		int tempRecv = _wtoi(vtProp.bstrVal);
		
		if(tempRecv > nByteRecv)
			nByteRecv = tempRecv;

		if(tempSend > nByteSend)
			nByteSend = tempSend;

		VariantClear(&vtProp);

		pclsObj->Release();
	}

	//clean
	pEnumerator->Release();
	if(!pclsObj) pclsObj->Release();


	return 0;   // Program successfully completed.
}

int WMIHelp::GetNetWorkTrafficPerSecTotal(int &nByteSend, int&nByteRecv)
{
	nByteSend = 0;
	nByteRecv = 0;

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t("SELECT * FROM Win32_PerfRawData_Tcpip_NetworkInterface"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);
	
	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property
		hr = pclsObj->Get(L"BytesSentPersec", 0, &vtProp, 0, 0);
		//wcout << " BytesSentPersec : " << vtProp.bstrVal << endl;
		int tempSend = _wtoi(vtProp.bstrVal);

		hr = pclsObj->Get(L"BytesReceivedPersec", 0, &vtProp, 0, 0);
		//wcout << " BytesReceivedPersec : " << vtProp.bstrVal << endl;
		int tempRecv = _wtoi(vtProp.bstrVal);
		
		if(tempRecv > nByteRecv)
			nByteRecv = tempRecv;

		if(tempSend > nByteSend)
			nByteSend = tempSend;

		VariantClear(&vtProp);

		pclsObj->Release();
	}

	//clean
	pEnumerator->Release();
	if(!pclsObj) pclsObj->Release();


	return 0;   // Program successfully completed.
}

int WMIHelp::GetCpuUserPercent(int &nCpuPsecent)
{
	nCpuPsecent = 0;

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;

	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t("SELECT * FROM Win32_PerfFormattedData_PerfProc_Process WHERE name!='Idle' AND name!='_Total'"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;


		// Get the value of the Name property
		hr = pclsObj->Get(L"name", 0, &vtProp, 0, 0);
		//wcout << " name : " << vtProp.bstrVal;

		hr = pclsObj->Get(L"PercentProcessorTime", 0, &vtProp, 0, 0);
		//wcout << " cpu : " << vtProp.bstrVal << endl;

		int tempCpuPercent = _wtoi(vtProp.bstrVal);


		nCpuPsecent += tempCpuPercent;
		
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	//clean
	pEnumerator->Release();
	if(!pclsObj) pclsObj->Release();


	return 0;   // Program successfully completed.


}


WMIHelp::WMIHelp(void)
{
	InitCom();
	InitWmi();
}


WMIHelp::~WMIHelp(void)
{
	// Cleanup
	// ========

	try
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();

	}
	catch (...)
	{
	}

	
}




