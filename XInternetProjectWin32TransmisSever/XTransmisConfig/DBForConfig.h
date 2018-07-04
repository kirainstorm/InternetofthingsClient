#pragma once


//256
struct ST_DEVICE_BASE
{
	//
	int dev_id;
	char dev_name[256-12];

	int time_start;
	int time_end;
};



struct ST_LIVE_STRUCT
{
	int nPlatformID; //>0
	ST_DEVICE_BASE dev[6];//dev_id==0表示该时间段无效
};

#define MAX_LIVE_STRUCT 8

struct ST_LIVE_STRUCT_ALL
{
	ST_LIVE_STRUCT all[MAX_LIVE_STRUCT];
};


class CDBForConfig
{
public:
	CDBForConfig();
	~CDBForConfig();
public:
	static CDBForConfig& Instance();
	vector<ST_DEVICE_BASE> m_stDevicves;

public:
	int GetAllDevices();

private:
	soci::session sql;
};

