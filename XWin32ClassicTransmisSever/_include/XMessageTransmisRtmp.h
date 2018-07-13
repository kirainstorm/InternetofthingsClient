#pragma once

#include "PlatformDefine.h"
#include "Pool.hpp"
#include "XMsgDefForTransmisProcess.h"
#include "RecycleService.hpp"
//�������̡�����rtmp����

extern volatile BOOL g_bServiceExit;
//
extern void __TransmisUpdataTick(char * devuuid);
class CMsgTransmisAccepter : public CXNetStreamData, public CXRecycleSession
{
public:
	CMsgTransmisAccepter(CXNetStream * pAcceptStream);
	~CMsgTransmisAccepter(void);
public:
	virtual void SessionDelete();
public:
	virtual void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);
private:
	void DoMsg();;
private:
	CXNetStream * m_pTcpStream;
	BOOL m_bCheckError;
	BOOL m_bNetIsError;
	CROSS_DWORD64 m_dwTick;
	emNetRecieveStep m_nRcvStep;
	TRANSMIN_PROC_DATA m_headRecv;
	TRANSMIN_PROC_DATA m_headSend;
};

class CMsgTransmisRtmpManagement : public CXNetAcceptStreamData
{
public:
	CMsgTransmisRtmpManagement();
	~CMsgTransmisRtmpManagement();
public:
	static CMsgTransmisRtmpManagement& Instance();
public:
	virtual void OnAcceptComplete(CXNetStream * pAcceptStream);

public:
	void AddRtmp(const char * user, const char * pwd,vector<string> devuuid_list_new, vector<string> &rtmpuuid_list); //����rtmpuuid
	void RemoveRtmp(vector<string> devuuid_list);
	void UpdataTick(char * devuuid);//����CTransmisRtmpManagement��ǰuuid��������
	void Start(int port = 15432);//15432Ϊ�ڲ���ַ������ǽ��Ҫ���ò�������������
	void Stop();

private:
	static void* WINAPI WorkerThread(void* param);
	void Worker();
private:
	CXTcpListener * m_hServer;
	HANDLE m_hThread;
	volatile BOOL m_hEvtWorkerStop;
	//
	CrossCriticalSection m_cs;
	map<string/*device uuid*/, ST_PROCESS_INFO*> g_mapProcess; //ֻ��ӣ�ֻ��ϵͳ�˳�ʱ��ɾ��ST_PROCESS_INFO
};
