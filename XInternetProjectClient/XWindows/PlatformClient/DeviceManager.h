#pragma once

// 
// #define PUBLISH_DEC_DBID_EX  (-10240)
 


#ifdef _PLATFORM_WINDOW

class CDeviceManager
{
public:
	CDeviceManager();
	~CDeviceManager();

	static CDeviceManager& Instance()
	{
		static CDeviceManager ins;
		return ins;
	}

	void ClearDevice();

public:
	vector<stDevice> m_DevList;


	BOOL GetDeviceByIndex(int i, stDevice &st);//取巧，m_DevList不变

public:
	//
	int GetCount(){ return m_DevList.size(); };
	//
	//BOOL GetDeviceByUuid(char * _devuuid, stDevice &st);//取巧，m_DevList不变
	//
	void SetDeviceOpenStatusByDeviceID(HTREEITEM item, BOOL bOpen);
	//
	BOOL AddDevice(stDevice st); 
	BOOL ModifyDevice(stDevice st);
	BOOL DeleteDevice(char * _devuuid);


private:
	CrossCriticalSection m_cs;
};

#endif