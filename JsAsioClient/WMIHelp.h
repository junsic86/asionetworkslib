#pragma once

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>

class WMIHelp
{
private:
	HRESULT hres;
	IWbemLocator *pLoc;
	IWbemServices *pSvc;

public:
	WMIHelp(void);
	~WMIHelp(void);
	int InitCom();
	int InitWmi();
	int ShowNetWorks(void);
};

