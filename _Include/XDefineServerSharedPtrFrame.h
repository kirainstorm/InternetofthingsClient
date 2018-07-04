#ifndef a_define_server_shareptr_frame_rthjmohgugfctdes
#define a_define_server_shareptr_frame_rthjmohgugfctdes

#include "XDefineMediaChannelStruct.h"

//--------------------------------------------------------------------------------------------------------

class CStreameFrame
{
public:
	CStreameFrame(void)
	{
		m_pFrame = nullptr;
		m_nTruthLen = 0;
	};
	~CStreameFrame(void)
	{
		if (m_pFrame) {
			FreeFrame(m_pFrame);
			m_pFrame = nullptr;
		}
	};

public:
	void SetFrame(const void* pData, int lLen, stFrameX_T stFrame)
	{
		UINT32 lFrameSize = sizeof(stFrameX_T) + lLen;
		//
		m_pFrame = AllocFrame(lFrameSize, &m_nExtendLen);
		//
		memcpy(m_pFrame, &stFrame, sizeof(stFrameX_T));
		//
		m_nTruthLen = lFrameSize;
		if (lLen > 0)
		{
			memcpy(&m_pFrame->data[0], pData, lLen);
		}
	};
	long GetTruthLen()//sizeof(pstFrameX)+datalen //����ʵ�ʳ���
	{
		return m_nTruthLen;
	};
	long GetExtendLen()//512���볤��
	{
		return m_nExtendLen;
	};
	stFrameX_T * GetBuffer()
	{
		return m_pFrame;
	};
private:
	//�ڴ���亯��
	//����ĳ�4096000û�з���й¶
	static stFrameX_T * AllocFrame(long lSize, long *lOutSize)
	{
		stFrameX_T * pFrame = nullptr;
		UINT  dwSize = (UINT)ceil(lSize*1.0 / 512.0) * 512;//�����ڴ���Ƭ 
#if 0
		BYTE* pMem = (PBYTE)VirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);//VirtualAlloc��4k�ڴ��ҳ
#else
		BYTE* pMem = new BYTE[dwSize];
		assert(pMem);
#endif
		assert(pMem);
		*lOutSize = dwSize; //����������512
		pFrame = (stFrameX_T *)pMem;
		return pFrame;
	};
	static void FreeFrame(stFrameX_T * pFrame)
	{
#if 0
		VirtualFree(pFrame, 0, MEM_RELEASE);
#else
		BYTE* pMem = (BYTE*)pFrame;
		delete[] pMem;
#endif
	};
private:
	stFrameX_T * m_pFrame;
	long m_nTruthLen;//����ʵ�ʳ���
	long m_nExtendLen;//512���볤��
};
//--------------------------------------------------------------------------------------------------------







/*
�������ṹ����ռ�ڴ��С��
Ϊ�ڴ��Ż���һ������ֽڶ��롣
�ֽڶ�����Ǳ����洢�ĵ�ַ�Ǳ�������Ч�ֽڶ���ֵ��������������
address%������Ч�ֽڶ���ֵ = 0;
�����ֽڶ���ֵ
1) �����������ֽڶ���ֵ
�����ͱ���
����char�����ݣ����������ֵΪ1������short��Ϊ2������int,float,double���ͣ����������ֵΪ4����λ�ֽڡ�
�ṹ���ͻ��������ͣ�
�����ֽڶ���ֵ�Ĵ�С�����Ա������������Ҫ�����Ч�ֽڶ���ֵ
2) ������Ҫ����ֽڶ���ֵ��
����������Ҫ����ֽڶ���ֵ
3) ������Ч���ֽڶ���ֵ��
�������ֽڶ���ֵ�ͱ�����Ҫ����ֽڶ���ֵ��ȡ��С�ġ�
��ʵ�ֽڶ����ϸ�ں;��������ʵ����أ���һ����ԣ���������׼��
1) �ṹ��������׵�ַ�ܹ��������������ͳ�Ա�Ĵ�С��������
2) �ṹ��ÿ����Ա����ڽṹ���׵�ַ��ƫ�������ǳ�Ա��С���߳�Ա���ӳ�Ա��С��ֻҪ�ó�Ա���ӳ�Ա������˵�����飬�ṹ��ȣ�����������������Ҫ���������ڳ�Ա֮���������ֽڣ���������ڶ����ṹ������ĵ�ַ�ռ䡣
3) �ṹ����ܴ�СΪ�ṹ�����������ͳ�Ա��С����������������Ҫ������������ĩһ����Ա֮���������ֽڡ�
ag.1
typedef struct bb
{
int id;             //[0]....[3]
double weight;      //[8].....[15]������������ԭ��
float height;      //[16]..[19],�ܳ�ҪΪ����������,����[20]...[23]����������ԭ��
}BB;

typedef struct aa
{
char name[2];     //[0],[1]
int  id;         //[4]...[7]��������������������ԭ��

double score;     //[8]....[15]��������
short grade;    //[16],[17]����������������
BB b;             //[24]......[47]��������������������ԭ��
}AA;

int main()
{
AA a;
cout<<sizeof(a)<<" "<<sizeof(BB)<<endl;
return 0;
}

ag.2
typedef union {char i; int k[2]; char c;} DATE;
struct data { int cat; DATE cow; char dog;} too;
DATE max;
printf("%d\n",sizeof(too));       //���Ϊ24

��ͬ�����г�Ա���� int�������ֽڶ���ֵΪ4����������Ĵ�С����Ϊdouble���ֽڶ���ֵΪ8��

*/

#endif
