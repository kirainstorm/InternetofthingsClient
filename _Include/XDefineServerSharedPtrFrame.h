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
	long GetTruthLen()//sizeof(pstFrameX)+datalen //数据实际长度
	{
		return m_nTruthLen;
	};
	long GetExtendLen()//512对齐长度
	{
		return m_nExtendLen;
	};
	stFrameX_T * GetBuffer()
	{
		return m_pFrame;
	};
private:
	//内存分配函数
	//这里改成4096000没有发现泄露
	static stFrameX_T * AllocFrame(long lSize, long *lOutSize)
	{
		stFrameX_T * pFrame = nullptr;
		UINT  dwSize = (UINT)ceil(lSize*1.0 / 512.0) * 512;//减少内存碎片 
#if 0
		BYTE* pMem = (PBYTE)VirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);//VirtualAlloc是4k内存分页
#else
		BYTE* pMem = new BYTE[dwSize];
		assert(pMem);
#endif
		assert(pMem);
		*lOutSize = dwSize; //磁盘扇区是512
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
	long m_nTruthLen;//数据实际长度
	long m_nExtendLen;//512对齐长度
};
//--------------------------------------------------------------------------------------------------------







/*
（三）结构体所占内存大小：
为内存优化，一般采用字节对齐。
字节对齐就是变量存储的地址是变量的有效字节对齐值的整数倍，即：
address%最终有效字节对齐值 = 0;
变量字节对齐值
1) 变量的自身字节对齐值
简单类型变量
对于char型数据，其自身对齐值为1，对于short型为2，对于int,float,double类型，其自身对齐值为4，单位字节。
结构类型或联合类型：
自身字节对齐值的大小是其成员中最大基本类型要求的有效字节对齐值
2) 编译器要求的字节对齐值：
编译器设置要求的字节对齐值
3) 最终有效的字节对齐值：
在自身字节对齐值和编译器要求的字节对齐值中取较小的。
其实字节对齐的细节和具体编译器实现相关，但一般而言，满足三个准则：
1) 结构体变量的首地址能够被其最宽基本类型成员的大小所整除。
2) 结构体每个成员相对于结构体首地址的偏移量都是成员大小或者成员的子成员大小（只要该成员有子成员，比如说是数组，结构体等）的整数倍，如有需要编译器会在成员之间加上填充字节；例如上面第二个结构体变量的地址空间。
3) 结构体的总大小为结构体最宽基本类型成员大小的整数倍，如有需要编译器会在最末一个成员之后加上填充字节。
ag.1
typedef struct bb
{
int id;             //[0]....[3]
double weight;      //[8].....[15]　　　　　　原则１
float height;      //[16]..[19],总长要为８的整数倍,补齐[20]...[23]　　　　　原则３
}BB;

typedef struct aa
{
char name[2];     //[0],[1]
int  id;         //[4]...[7]　　　　　　　　　　原则１

double score;     //[8]....[15]　　　　
short grade;    //[16],[17]　　　　　　　　
BB b;             //[24]......[47]　　　　　　　　　　原则２
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
printf("%d\n",sizeof(too));       //结果为24

共同体中有成员类型 int，所以字节对齐值为4，忽略数组的大小，若为double，字节对齐值为8。

*/

#endif
