// XWin32TransmisSever.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "TransmisClient.hpp"
#include "TransmisServer.hpp"
#include "DatabaseForClassicTransmis.h"



void run()
{

	CTransmisServer * pSevr = new CTransmisServer(11100);
	pSevr->Start();
	CMsgTransmisRtmpManagement::Instance().Start();
	while (true)
	{

		if (CDatabaseForClassicTransmis::Instance().IsSqlStatusError())
		{
			CDatabaseForClassicTransmis::Instance().OpenSql();
		}
		//
		vector<st_database_Transmis_stream_rtmp *> v;
		CDatabaseForClassicTransmis::Instance().GetTransmisDevices(v);
		//
		CMsgTransmisRtmpManagement::Instance().UpdataProcess(v);
		//
		Sleep(1000);
		//
		for (vector<st_database_Transmis_stream_rtmp *>::iterator it = v.begin(); it != v.end(); it++)
		{
			delete *it;
		}
		v.clear();
	}
	pSevr->Stop();
	CMsgTransmisRtmpManagement::Instance().Stop();
	delete pSevr;
}

int _tmain(int argc, _TCHAR* argv[])
{

	int a = sizeof(ST_PROCESS_INFO);
	int b = sizeof(ST_PROCESS_INFO_EEEEEERS);






	run();




	return 0;
}

