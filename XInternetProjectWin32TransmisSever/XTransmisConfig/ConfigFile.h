#pragma once

#include "DBForConfig.h"

class CConfigFile
{
public:
	CConfigFile();
	~CConfigFile();

public:
	static CConfigFile& Instance();


public:
	//дTransmisSever��info.txt
	//BOOL WriteTransmisSeverInfo(char * info_txt);



	//��дXTransmisConfig��������Ϣ
	void ReadConfigFile();
	void SaveConfigFile();


	ST_LIVE_STRUCT_ALL m_stAllConfig;
public:
	
	CrossCriticalSection m_cs;

};

