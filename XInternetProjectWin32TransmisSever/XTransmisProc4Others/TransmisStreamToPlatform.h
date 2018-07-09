#ifndef TransmisStreamToPlatform_2133234451nj23gh2q232j
#define TransmisStreamToPlatform_2133234451nj23gh2q232j



//--------------------------------------------------------
//
//
//	��������ģ��һ̨�豸����������ӣ����������ݵ��շ�
//
//
//--------------------------------------------------------

#include "DataTypeDevice.h"
#include "XDes.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/pool/pool.hpp>
#include "mediainterfacedefine.h"


#define MAX_STRUCT_LEN_4_TRANSMIS_DATA (2*1024*1024)
struct MyStruct4TransmisData
{
	char buffer[MAX_STRUCT_LEN_4_TRANSMIS_DATA - sizeof(int)];//rqst_head+data
	int nSendLen;
};
enum emTransmisStreamToPlatformSendStep
{
	TRANSMIS_NET_SEND_STEP_SLEEP = 0,
	TRANSMIS_NET_SEND_STEP_SEND
};

class CTransmisStreamToPlatform :public CXNetStreamData
{
public:
	CTransmisStreamToPlatform();
	~CTransmisStreamToPlatform();

public:
	static CTransmisStreamToPlatform& Instance();

public:
	//of CXNetStreamData
	virtual void DeleteStreamData();
	virtual void OnPacketCompleteNetStreamData(int32_t dwCompleteBytes, int32_t bRecieve);

public:
	//for int _tmain(int argc, _TCHAR* argv[])
	void SetPlatformInfo(char * ip , int port ,int id);

public:
	//for XTransmisDeviceInterface
	BOOL GetError();//�Ƿ��д������
	BOOL LoginPlatform();
	BOOL LogoutPlatform();

	//�����������̵���1��16000��8000��2048��aac�����򲻷�����Ƶ
	BOOL SendMessgeToPlatform(emFREAM_TYPE_DEFINE type, char * data, int datalen, int framerate, int width, int height, long long tick);

	//����ģ�ƽ̨��ƽ̨
	BOOL SendMessgeToPlatformFromPlatform(ST_SERVER_HEAD head, const char* pStreamData, int nLen);
public:
	char m_szIp[32];
	int m_nPort;
	int m_nID;
	int m_nSessionID;
	CROSS_DWORD64 m_dTickHearbeat;//
	BOOL m_bLoginError;
private:
	//
	CXNetStream* m_pTcpStream;
	CrossCriticalSection m_csTcp;
	BOOL m_bStreamIsError;
	//recv

	emNetRecieveStep m_nRcvStep;
	rqst_head m_head;
	int m_dataLen;
	unsigned char * m_szBuffer;
	void DoMsg();
	//
	emTransmisStreamToPlatformSendStep m_nSendStep;
	boost::pool<> m_plMsg;//�ڴ�أ���Ϣ����
	list<MyStruct4TransmisData *> m_msgList;
	CrossCriticalSection m_csMsg;
private:
	CROSS_THREAD_HANDLE	m_hWorkerThread;
	BOOL m_hEvtWorkerStop;
	static int WorkerThread(void*);
	void Worker();



};

#endif