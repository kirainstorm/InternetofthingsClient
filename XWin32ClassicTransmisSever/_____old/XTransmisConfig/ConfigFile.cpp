#include "stdafx.h"
#include "ConfigFile.h"


CConfigFile::CConfigFile()
{
	memset(&m_stAllConfig, 0, sizeof(m_stAllConfig));
}


CConfigFile::~CConfigFile()
{
}

CConfigFile& CConfigFile::Instance()
{
	static CConfigFile inc;
	return inc;
}

// 
// //写TransmisSever的info.txt
// BOOL CConfigFile::WriteTransmisSeverInfo(char * info_txt)
// {
// 
// 	BOOL ret = FALSE;
// 
// 	HANDLE hSemaphore = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("Global\\WriteTransSeverEvent"));//_T("Global\\WriteTransSeverEvent")
// 	if (NULL == hSemaphore)
// 	{
// 		
// 		TRACE("************************** 主进程挂了 *********************\n");
// 		return FALSE;
// 	}
// 
// 	do 
// 	{
// 		if (WAIT_OBJECT_0 != WaitForSingleObject(hSemaphore, 2*1000))
// 		{
// 			TRACE("**22************************ 主进程挂了 ********************* \n");
// 			break;
// 		}
// 
// 		CFile file;//要操作的文件
// 		UINT size;//要操作的文件大小
// 		UINT readsize;//file.Read的文件大小
// 
// 
// 		//CFile::modeCreate 直接构建创建一个新文件。如果该文件已经存在，则文件内容自动被清零
// 		//\\output32
// 		if (file.Open(L"c:\\TransmisSever\\info.txt", CFile::modeCreate | CFile::modeReadWrite))
// 		{
// 			file.Write(info_txt, strlen(info_txt));// nMaxSize);
// 			file.Close();
// 		}
// 		else
// 		{
// 			TRACE("**22************************ 7 ********************* \n");
// 
// 		}
// 
// 		ret = TRUE;
// 
// 	} while (0);
// 
// 	
// 	SetEvent(hSemaphore);
// 	CloseHandle(hSemaphore);
// 
// 	return ret;
// }



//读写XTransmisConfig的配置信息
void CConfigFile::ReadConfigFile()
{
	TCHAR szFilePath[MAX_PATH];
	ZeroMemory(szFilePath, MAX_PATH);
	if (GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
		PathRemoveFileSpec(szFilePath);
	}
	CString strFileConfig = _T("");
	strFileConfig = szFilePath;
	strFileConfig += _T("\\info.xtu");




	CFile file;
	if (file.Open(strFileConfig, CFile::modeRead))
	{
		file.Read(&m_stAllConfig, sizeof(m_stAllConfig));
		file.Close();
	}
}
void CConfigFile::SaveConfigFile()
{
	TCHAR szFilePath[MAX_PATH];
	ZeroMemory(szFilePath, MAX_PATH);
	if (GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
		PathRemoveFileSpec(szFilePath);
	}
	CString strFileConfig = _T("");
	strFileConfig = szFilePath;
	strFileConfig += _T("\\info.xtu");


	CFile file;
	if (file.Open(strFileConfig, CFile::modeCreate | CFile::modeReadWrite))
	{
		file.Write(&m_stAllConfig, sizeof(m_stAllConfig));
		file.Close();
	}
}