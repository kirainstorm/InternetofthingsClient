#ifndef DES_LKJSADFJHUJSDFJ567342P8KJSD
#define DES_LKJSADFJHUJSDFJ567342P8KJSD


#include "PlatformDefine.h"
#include "XCross.h"




//#define DBconnectString "DSN=DBPlatCenter;Uid=sa;Pwd=123456"


//只做8位

// #ifdef _PLATFORM_ANDROID
// 
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <jni.h>
// #include <android/log.h>
// #define LOG_TAG "utilityCode"
// #define Log(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
// #define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
// #define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
// #endif

#include <math.h>
//openssl
#include <openssl/des.h>
/*
#ifdef _DEBUG
#pragma comment( lib, "libeay32MDd.lib" )
#pragma comment( lib, "ssleay32MDd.lib" )
#else
#pragma comment( lib, "libeay32MD.lib" )
#pragma comment( lib, "ssleay32MD.lib" )
#endif
*/

/*
// C prototype : void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
// [IN] pbSrc - 字符串
// [IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value:
// remarks : 将字符串转化为16进制数
*/
inline void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
{
	char h1, h2;
	BYTE s1, s2;
	int i;

	for (i = 0; i < nLen; i++)
	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		pbDest[i] = s1 * 16 + s2;
	}
}

/*
// C prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 存放目标字符串
// [IN] pbSrc - 输入16进制数的起始地址
// [IN] nLen - 16进制数的字节数
// return value:
// remarks : 将16进制数转化为字符串
*/
inline void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
{
	char ddl, ddh;
	int i;

	for (i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i * 2] = ddh;
		pbDest[i * 2 + 1] = ddl;
	}

	pbDest[nLen * 2] = '\0';
}



#define DES_KEY ("a#kh86I^")
const int des_keylen = 8;

inline void XDESEncode(const char *pbSrc, unsigned int ses, char *pbDest)
{
	DES_cblock key;
	DES_key_schedule schedule;
	const_DES_cblock input;
	DES_cblock output;
	//DES_cblock ivec;

	//Log("XDESEncode 111\n");

	//key
	char _szkey[256] = { 0 };

	//data
	char _data[256] = { 0 };

	//session
	//char _ses[256] = { 0 };


	//temp
	unsigned char temp[256] = { 0 };

	//Log("XDESEncode 222\n");
	memcpy(_szkey, DES_KEY, des_keylen);
	memcpy(_data, pbSrc, strlen(pbSrc));



	//memcpy(_data + strlen(pbSrc), &ses, sizeof(UINT32));//直接拷贝&ses可能出现0x01等
	//int datalen = strlen(pbSrc) + sizeof(UINT32);//+ session

	char tempSes[32] = { 0 };
	CROSS_SPRINTF(tempSes, "%d", ses);

    
	//Log("XDESEncode 333\n");
	//int a = atol(tempSes);

	char szM[32] = { 0x08 };
	memcpy(_data + strlen(pbSrc), szM, 1);
	memcpy(_data + strlen(pbSrc) + 1, tempSes, strlen(tempSes));
	int datalen = (int)strlen(pbSrc) + (int)strlen(tempSes) + 1;
	//Log("XDESEncode 444\n");
	/* 分析补齐明文所需空间及补齐填充数据 */
	//int data_len = strlen(pbSrc);
	int data_rest = datalen % 8;

	int len = datalen;

	if (data_rest > 0)
	{
		len += (8 - data_rest);
		// 
		int ch = 8 - data_rest;
		//	
		// 	memset(src, 0, len);
		// 	memcpy(src, data, data_len);

		switch (ch)
		{
		case 1:
			memset(_data + datalen, 0x01, ch);
			break;
		case 2:
			memset(_data + datalen, 0x02, ch);
			break;
		case 3:
			memset(_data + datalen, 0x03, ch);
			break;
		case 4:
			memset(_data + datalen, 0x04, ch);
			break;
		case 5:
			memset(_data + datalen, 0x05, ch);
			break;
		case 6:
			memset(_data + datalen, 0x06, ch);
			break;
		case 7:
			memset(_data + datalen, 0x07, ch);
			break;
		default:
			break;
		}


	}
	//Log("XDESEncode 555\n");



	if (des_keylen == 8)
	{
		memcpy(key, _szkey, des_keylen);
		DES_set_key_unchecked(&key, &schedule);
	}
	//Log("XDESEncode 666\n");
	//单DES算法
	if (des_keylen == 8)
	{
		//datalen = (DWORD)ceil(datalen*1.0 / 8) * 8;
		int i;
		for (i = 0; i < len / 8; i++)
		{

			// 				memset(tmp, 0, 8);
			// 				memset(in, 0, 8);
			// 				memset(out, 0, 8);
			// 				memcpy(tmp, src + 8 * i, 8);
			// 
			// 				/* 加密 */
			// 				DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)in, &ks, &ks2, &ks3, DES_ENCRYPT);
			// 				/* 解密 */
			// 				DES_ecb3_encrypt((const_DES_cblock*)in, (DES_cblock*)out, &ks, &ks2, &ks3, DES_DECRYPT);
			// 				/* 将解密的内容拷贝到解密后的明文 */
			// 				memcpy(dst + 8 * i, out, 8);



			memset(input, 0, 8);
			memcpy(input, _data + i * 8, 8);


			DES_ecb_encrypt(&input, &output, &schedule, DES_ENCRYPT);

			memcpy(temp + i * 8, output, 8);
		}
	}
	//Log("XDESEncode 777\n");
	//unsigned char m_result[256] = { 0 };
	//HexToStr(pbDest, temp, datalen);
	HexToStr((unsigned char *)pbDest, temp, len);
	//TRACE(m_result);
}

inline void XDESDecode(const char *pbSrc, char *pbDest, unsigned int &ses)
{

	DES_cblock key;
	DES_key_schedule schedule;
	const_DES_cblock input;
	DES_cblock output;
	//DES_cblock ivec;

	//key
	unsigned char _szkey[256] = { 0 };

	//data
	int datalen = (int)strlen(pbSrc);
	unsigned char _data[256] = { 0 };

	//temp
	unsigned char temp[256] = { 0 };
	if ((datalen %(2*8)) != 0)
	{
		ses = 0;
		return;
	}

	memcpy(_szkey, DES_KEY, des_keylen);
	StrToHex(_data, (unsigned char*)pbSrc, datalen / 2);



	if (des_keylen == 8)
	{
		memcpy(key, _szkey, des_keylen);
		DES_set_key_unchecked(&key, &schedule);
	}


	int nLen = 0;

	for (int i = 0; i < datalen / 2 / 8; i++)
	{
		memcpy(input, _data + i * 8, 8);
		DES_ecb_encrypt(&input, &output, &schedule, DES_DECRYPT);


		if (output[7] == 0x01)
		{
			nLen += 7;
			memcpy(temp + i * 8, output, 7);
		}
		else if ((output[6] == 0x02) && (output[7] == 0x02))
		{
			nLen += 6;
			memcpy(temp + i * 8, output, 6);
		}
		else if ((output[5] == 0x03) && (output[6] == 0x03) && (output[7] == 0x03))
		{
			nLen += 5;
			memcpy(temp + i * 8, output, 5);
		}
		else if ((output[4] == 0x04) && (output[5] == 0x04) && (output[6] == 0x04) && (output[7] == 0x04))
		{
			nLen += 4;
			memcpy(temp + i * 8, output, 4);
		}
		else if ((output[3] == 0x05) && (output[4] == 0x05) && (output[5] == 0x05) && (output[6] == 0x05) && (output[7] == 0x05))
		{
			nLen += 3;
			memcpy(temp + i * 8, output, 3);
		}
		else if ((output[2] == 0x06) && (output[3] == 0x06) && (output[4] == 0x06) && (output[5] == 0x06) && (output[6] == 0x06) && (output[7] == 0x06))
		{
			nLen += 2;
			memcpy(temp + i * 8, output, 2);
		}
		else if ((output[1] == 0x07) && (output[2] == 0x07) && (output[3] == 0x07) && (output[4] == 0x07) && (output[5] == 0x07) && (output[6] == 0x07) && (output[7] == 0x07))
		{
			nLen += 1;
			memcpy(temp + i * 8, output, 1);
		}
		else
		{
			nLen += 8;
			memcpy(temp + i * 8, output, 8);
		}

	}


	



	//memcpy(pbDest, temp, nLen - sizeof(UINT32));
	//memcpy(&ses, temp + nLen - sizeof(UINT32), sizeof(UINT32));

	char szM[32] = { 0x08 };
	char tempSes[32] = { 0 };
	char * pTemp = strstr((char *)temp, szM);

	if (NULL == pTemp)
	{
		ses = 0;
	}
	else
	{
		memcpy(pbDest, temp, nLen - strlen(pTemp));

		memcpy(tempSes, pTemp + 1, strlen(pTemp) - 1);
		ses = atoi(tempSes);
	}


	int a = 0;

	return;
}
















#endif
