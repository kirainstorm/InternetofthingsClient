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
	//写TransmisSever的info.txt
	//BOOL WriteTransmisSeverInfo(char * info_txt);



	//读写XTransmisConfig的配置信息
	void ReadConfigFile();
	void SaveConfigFile();


	ST_LIVE_STRUCT_ALL m_stAllConfig;
public:
	
	CrossCriticalSection m_cs;

};

