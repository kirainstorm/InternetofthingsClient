#include "stdafx.h"
//#include "VideoSet.h"
#include "DecodeCard_DS.h"


CDecodeCard_DS::CDecodeCard_DS()
{
	m_nChannelCount = 0;
	m_nErrorCode = 0;
	HRESULT hr = DirectDrawEnumerateExA(DDEnumCallbackEx, this, DDENUM_ATTACHEDSECONDARYDEVICES | DDENUM_NONDISPLAYDEVICES);
	if (hr != DD_OK)
	{
		m_nErrorCode = -1;
	}
	Sleep(1000);
}

CDecodeCard_DS::~CDecodeCard_DS()
{
#if 0
	map<DWORD64, stDisplaySet *>::iterator itVS;
	for (itVS = m_mapDisplay.begin(); itVS != m_mapDisplay.end(); itVS++)
	{
		for (size_t i = 0; i < MAX_DISPALY_SET; i++)
		{
			if (itVS->second->play[i])
			{
				itVS->second->play[i]->Release();
				itVS->second->play[i] = NULL;
			}
		}
		delete itVS->second;
	}
	m_mapDisplay.clear();

#endif
	m_nChannelCount = 0;
	m_nErrorCode = 0;
}

int CDecodeCard_DS::GetDevMode(CStringA DevName, DEVMODEA& dm)
{
	static int nMainDispCount = 0;
	dm.dmSize = sizeof(DEVMODEA);
	if (::EnumDisplaySettingsA(DevName, ENUM_REGISTRY_SETTINGS, &dm))
	{
#if 1
		if (dm.dmOrientation == 0)
		{
			/*if (m_bIsMainDisplay && nMainDispCount == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}*/
			return -1;
		}
#endif
	}
	return 0;
}

BOOL WINAPI CDecodeCard_DS::DDEnumCallbackEx(GUID FAR *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm)
{
	CDecodeCard_DS *pDCDS = (CDecodeCard_DS*)lpContext;
	if (lpContext == NULL)
	{
		return FALSE;
	}
	if (lpGUID != NULL)
	{
		DEVMODEA dm;
		if (!pDCDS->GetDevMode(lpDriverName, dm))
		{
			pDCDS->m_channelset[pDCDS->m_nChannelCount].guid = *lpGUID;
			pDCDS->m_channelset[pDCDS->m_nChannelCount].dm = dm;
			memset(pDCDS->m_channelset[pDCDS->m_nChannelCount].name, 0, sizeof(pDCDS->m_channelset[pDCDS->m_nChannelCount].name));
			memcpy(pDCDS->m_channelset[pDCDS->m_nChannelCount].name, lpDriverName, strlen(lpDriverName));
			pDCDS->m_channelset[pDCDS->m_nChannelCount].channel = pDCDS->m_nChannelCount;
			pDCDS->m_channelset[pDCDS->m_nChannelCount].ok = FALSE;
			pDCDS->m_nChannelCount++;
		}
	}
	return TRUE;
}
long CDecodeCard_DS::GetChannelCount(long * count, stDisplayInfo &st)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}

	if (m_nChannelCount <= 0)
	{
		m_nErrorCode = -1;
		return -2;
	}

	memset(&st, 0, sizeof(st));


	*count = m_nChannelCount;
	for (int i = 0; i < m_nChannelCount; i++)
	{
		st.play[i].guid = m_channelset[i].guid;
		st.play[i].channel = i;
	}
	return 0;
}
#ifdef TYPE_FOR_CONFIG


long CDecodeCard_DS::InitChannelDecoder()
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}

	for (int i = 0; i < m_nChannelCount; i++)
	{
		int w = m_channelset[i].dm.dmPelsWidth;
		int h = m_channelset[i].dm.dmPelsHeight;
		CRect rcOutRect(0, 0, w, h);
		//CRect cDisplayRect(w / 2, 0, w , h / 2);
		m_pDisplay[i] = new CDisplay(m_channelset[i].guid, m_channelset[i].dm, rcOutRect, 1, 1, 0, 0);




		m_pDisplay[i]->Init(w, h);
		m_pDisplay[i]->Refresh();
	}


	return 0;
}
long CDecodeCard_DS::DestroyChannelDecoder()
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	for (int i = 0; i < m_nChannelCount; i++)
	{
		m_pDisplay[i]->Release();
	}

	return 0;
}
long CDecodeCard_DS::FreshChannel(GUID guid)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	for (int i = 0; i < m_nChannelCount; i++)
	{
		if (CompareGUID(m_pDisplay[i]->GetGUID(), guid))
		{
			m_pDisplay[i]->Flash();
		}
	}
	return 0;
}

long CDecodeCard_DS::SetChannelIndex(GUID guid, int nIndex)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}

	for (int i = 0; i < m_nChannelCount; i++)
	{
		if (CompareGUID(m_channelset[i].guid , guid))
		{
			m_channelset[i].channel = nIndex;
		}
	}

	return 0;
}
long CDecodeCard_DS::SaveChannelGuidMapping()
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	char m_szFileName[256] = {0};
	char strModulePath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
	std::string str(strModulePath);
	size_t nCount = str.find_last_of("\\");
	if (nCount < 0)
		nCount = str.find_last_of("/");

	if (nCount >= 0)
		str = str.substr(0, nCount + 1);

	str.append("\\dec.card.mapping");
	memset(m_szFileName, 0, sizeof(m_szFileName));
	strcpy_s(m_szFileName, str.c_str());



	FILE * file;
	fopen_s(&file, m_szFileName, "wb");//wb 只写打开或新建一个二进制文件，若文件存在则文件长度清为0；只允许写数据。
	if (file == NULL)
	{
		return -1;
	}

	for (int i = 0; i < m_nChannelCount; i++)
	{

		fwrite(&m_channelset[i], 1, sizeof(m_channelset[i]), file);
	}

	fclose(file);

	return 0;

	/*
			FILE * file;
		fopen_s(&file, m_szFileName, "r");//r 读
		if (file == NULL)
		{
			strcpy_s(t->loginuser, "admin");

			strcpy_s(t->picPath, "D:\\");
			strcpy_s(t->recPath, "D:\\");

			t->spiltScreen = 4;
			t->recTime = 3;
			t->recSize = 30;
			t->diskSpace = 4;
			t->autoRemain = 1;

			t->lg = 0;
			t->spiltScreen = 4;

			Set(*t);
			return 0;
		}

		fread(t, 1, sizeof(Ssy_Config_Struct), file);

		fclose(file);
	*/
}
#endif






#ifdef TYPE_FOR_SERVER
long  CDecodeCard_DS::LoadChannelGuidMapping(int hang, int lie)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	m_hang = hang;
	m_lie = lie;



	stChannelParam channelsetTmp[MAX_DECODE_CHANNEL];

	char m_szFileName[256] = {0};
	char strModulePath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
	std::string str(strModulePath);
	size_t nCount = str.find_last_of("\\");
	if (nCount < 0)
		nCount = str.find_last_of("/");

	if (nCount >= 0)
		str = str.substr(0, nCount + 1);

	str.append("\\dec.card.mapping");
	memset(m_szFileName, 0, sizeof(m_szFileName));
	strcpy_s(m_szFileName, str.c_str());



	FILE * file;
	fopen_s(&file, m_szFileName, "r+");//wb 只写打开或新建一个二进制文件，若文件存在则文件长度清为0；只允许写数据。
	if (file == NULL)
	{
		return -1;
	}

	for (int i = 0; i < m_nChannelCount; i++)
	{
		int n = fread(&channelsetTmp[i], 1, sizeof(channelsetTmp[i]), file);
		if (n < sizeof(channelsetTmp[i]))
		{
			fclose(file);
			return -1;
		}
	}
	fclose(file);


	//----------------------------------------------
	for (int i = 0; i < m_nChannelCount; i++)
	{
		for (int t = 0; t < m_nChannelCount; t++)
		{
			if (CompareGUID(m_channelset[i].guid, channelsetTmp[t].guid))
			{
				m_channelset[i].ok = TRUE;
				m_channelset[i].channel = channelsetTmp[t].channel;
				break;
			}
		}
	}

	//---------------------------------
	for (int i = 0; i < m_nChannelCount; i++)
	{
		if (!m_channelset[i].ok)
		{
			return -1;
		}
	}
	//-----------------------------------

	return 0;
}

int CDecodeCard_DS::GetChannelIndex(int ch)
{
	for (int i = 0; i < m_nChannelCount; i++)
	{
		if (m_channelset[i].channel == ch)
		{
			return i;
		}
	}

	return -1;
}
long CDecodeCard_DS::InitPlayView(stPlayView stView)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}

	memcpy(&m_stPlayView, &stView, sizeof(m_stPlayView));

	for (int i = 0; i < m_stPlayView.count; i++)
	{

		switch (m_stPlayView.param[i].type)
		{
		case SCREEN_TYPE_SINGLE:
		{
			int ch = m_stPlayView.param[i].channel;
			int m = GetChannelIndex(ch);


			int w = m_channelset[m].dm.dmPelsWidth;
			int h = m_channelset[m].dm.dmPelsHeight;

			CRect rcOutRect(0, 0, w, h);
			CDisplay * p = new CDisplay(m_channelset[m].guid, m_channelset[m].dm, rcOutRect,1,1,0,0);
			p->Init(w, h);
			p->Refresh();
			m_stPlayView.param[i].m_mapDisplay.push_back(p);
		}
			break;
		case SCREEN_TYPE_FOURE:
		{
			int ch = m_stPlayView.param[i].channel;
			int subch = m_stPlayView.param[i].subchannel;
			int m = GetChannelIndex(ch);


			int w = m_channelset[m].dm.dmPelsWidth;
			int h = m_channelset[m].dm.dmPelsHeight;
			switch (subch)
			{
			case 0:
			{
				CRect rcOutRect(0, 0, w / 2, h / 2);
				CDisplay * p = new CDisplay(m_channelset[m].guid, m_channelset[m].dm, rcOutRect, 1, 1, 0, 0);
				DWORD dw = GetTickCount();
				p->Init(w, h);
				int ff = GetTickCount() - dw;
				p->Refresh();
				int zz = GetTickCount() - dw - ff;
				m_stPlayView.param[i].m_mapDisplay.push_back(p);
			}
			break;
			case 1:
			{
				CRect rcOutRect(w / 2, 0, w , h / 2);
				CDisplay * p = new CDisplay(m_channelset[m].guid, m_channelset[m].dm, rcOutRect, 1, 1, 0, 0);
				p->Init(w, h);
				p->Refresh();
				m_stPlayView.param[i].m_mapDisplay.push_back(p);
			}
				break;
			case 2:
			{
				CRect rcOutRect(0, h / 2, w / 2, h );
				CDisplay * p = new CDisplay(m_channelset[m].guid, m_channelset[m].dm, rcOutRect, 1, 1, 0, 0);
				p->Init(w, h);
				p->Refresh();
				m_stPlayView.param[i].m_mapDisplay.push_back(p);
			}
				break;
			case 3:
			{
				CRect rcOutRect(w / 2, h / 2, w , h );
				CDisplay * p = new CDisplay(m_channelset[m].guid, m_channelset[m].dm, rcOutRect, 1, 1, 0, 0);
				p->Init(w, h);
				p->Refresh();
				m_stPlayView.param[i].m_mapDisplay.push_back(p);
			}
				break;
			default:
				break;
			}


		}
			break;
		case SCREEN_TYPE_UNITE_SINGLE:
		{
			//int ch = m_stPlayView.param[i].channel;
			//int subch = m_stPlayView.param[i].subchannel;
#if 1
			int u = 0, v = 0 ,k=0, z=-1;
			for (size_t m = 0; m < 128; m++)
			{
				if (m_stPlayView.param[i].otherchannel[m])
				{

					k++;//计算总共占用了多少个屏幕
					if (z==-1)
					{
						z = m;//拼屏占用的第一个窗口是墙的第几个
					}
				}
			}


			int h = z / m_lie;//判断在第几行
			int l = z % m_lie;//判断在第几列

			for (int m = z; m < m_lie*(h + 1); m++)
			{
				if (m_stPlayView.param[i].otherchannel[m])
				{
					v++;//拼屏的列有几个
				}
			}
			for (int m = h; m < m_hang + 1; m++)
			{
				if (m_stPlayView.param[i].otherchannel[m*3+l])
				{
					u++;//拼屏的行有几个
				}
			}

#endif

			//int m = GetChannelIndex(ch);
			//int w = m_channelset[m].dm.dmPelsWidth;
			////int h = m_channelset[m].dm.dmPelsHeight;

			int s = 0;
			for (size_t k = 0; k < 128; k++)
			{
				if (m_stPlayView.param[i].otherchannel[k])
				{
					int m = GetChannelIndex(k);
					int w = m_channelset[m].dm.dmPelsWidth;
					int h = m_channelset[m].dm.dmPelsHeight;

					CRect rcOutRect(0, 0, w, h);
					CDisplay * p = new CDisplay(m_channelset[m].guid, m_channelset[m].dm, rcOutRect, 
						u, v, 
						s%v,
						s/v
						);

					s++;

					p->Init(w, h);
					p->Refresh();
					m_stPlayView.param[i].m_mapDisplay.push_back(p);
				}
			}





		}
			break;
		default:
			break;
		}


	}

	return 0;
}
long CDecodeCard_DS::DestroyPlayView()
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}

	return 0;
}

long CDecodeCard_DS::InputData(stViewParam stView, const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight, const wchar_t* szOutText, int nTextLen)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}


	for (int i = 0; i < m_stPlayView.count; i++)
	{
		if (stView.tick == m_stPlayView.param[i].tick)
		{

			for (size_t n = 0; n < m_stPlayView.param[i].m_mapDisplay.size(); n++)
			{
				m_stPlayView.param[i].m_mapDisplay[n]->InputData(pData, lDataLen, lDataFmt, dwWidth, dwHeight);
			}
			for (size_t n = 0; n < m_stPlayView.param[i].m_mapDisplay.size(); n++)
			{
				m_stPlayView.param[i].m_mapDisplay[n]->render();
			}
			break;
		}
	}


	return 0;
}
long CDecodeCard_DS::FreshData(stViewParam stView)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}

	for (int i = 0; i < m_stPlayView.count; i++)
	{
		if (stView.tick == m_stPlayView.param[i].tick)
		{
			for (size_t n = 0; n < m_stPlayView.param[i].m_mapDisplay.size(); n++)
			{
				m_stPlayView.param[i].m_mapDisplay[n]->Refresh();
			}
		}
	}

	return 0;
}
#endif
/*
long CDecodeCard_DS::GetChannelCount()
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	return m_nChannelCount;
}
long CDecodeCard_DS::FreshChannel(long lChannel)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	return 0;
}

GUID CDecodeCard_DS::GetChannelGuid(long lChannel)
{
	
	return m_channelset[lChannel].guid;
}

long CDecodeCard_DS::SetChannelGuidMapping(stChannelMapping stMap)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	return 0;
}

long CDecodeCard_DS::InitDecoder(stPlayView stView)
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	return 0;
}
long CDecodeCard_DS::DestroyDecoder()
{
	if (0 != m_nErrorCode)
	{
		return -1;
	}
	return 0;
}
long CDecodeCard_DS::InputData(long lChannel, long lSubChannel, const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight, const wchar_t* szOutText, int nTextLen)
{
#if 0
	if (m_mapVideoSet[lChannel] == NULL)
	{
		return NULL;
	}
	m_mapVideoSet[lChannel]->AddOutInfoText(lMatrix, szOutText, nTextLen);
	CVideoSet* pVSet = NULL;
	IRender* pResult = NULL;

	long i = IsChannelSet(lChannel);
	switch(i)
	{
	case -1:
		{
			SetDisplay(lChannel, dwWidth, dwHeight);
			break;
		}
	case -2:
		{
			m_InLock.Lock();
			pResult = m_mapVideoSet[lChannel]->InputData(lMatrix, pData, lDataLen, lDataFmt, dwWidth, dwHeight);
			m_InLock.Unlock();
			break;
		}
	default :
		{
			m_InLock.Lock();
			for (long j = 0; m_nDisplayUnite[i][j] != -1; j++)
			{
				m_mapVideoSet[m_nDisplayUnite[i][j]]->InputData(lMatrix, pData, lDataLen, lDataFmt, dwWidth, dwHeight);
			}
			m_InLock.Unlock();
			break;
		}
	}

	return pResult;

#else
	return 0;
#endif
}


long CDecodeCard_DS::FreshData(long lChannel, long lMatrix)
{
	if (m_mapVideoSet[lChannel] == NULL)
	{
		return DECODE_INPUTPARAM_ERROR;
	}
	m_mapVideoSet[lChannel]->FreshData(lMatrix);
	return DECODE_SUCCEED;
}




long CDecodeCard_DS::IsChannelSet(long lChannel)
{
	m_UniteLock.Lock();

	map<long, long>::iterator it;
	it = m_mapChannelIsSet.find(lChannel);
	if (it == m_mapChannelIsSet.end())
	{
		for (int i = 0; i < MAX_UNITE_COUNT; i++)
		{
			if (m_nDisplayUnite[i][0] == -1)
			{
				break;
			}
			for (int j = 0; j < MAX_UNITECHILD_COUNT; j++)
			{
				if (m_nDisplayUnite[i][j] == lChannel)
				{
					m_UniteLock.Unlock();

					return -1;
				}
			}
		}
		m_UniteLock.Unlock();

		return -2;
	}
	m_UniteLock.Unlock();

	return it->second;
}

void CDecodeCard_DS::SetDisplay(long lChannel, DWORD dwWidth, DWORD dwHeight)
{
	m_UniteLock.Lock();

	int nDisplayCount = 0;
	long lFindChannel = -1;
	for (int i = 0; i < MAX_UNITE_COUNT; i++)
	{
		if (m_nDisplayUnite[i][0] == -1)
		{
			break;
		}
		for (int j = 0; j < MAX_UNITECHILD_COUNT; j++)
		{
			if (m_nDisplayUnite[i][j] == lChannel)
			{
				for (; m_nDisplayUnite[i][nDisplayCount] != -1; nDisplayCount++);
				for (int n = 0; m_nDisplayUnite[i][n] != -1; n++)
				{
					m_mapVideoSet[m_nDisplayUnite[i][n]]->SetDisplayUnite(nDisplayCount, n, dwWidth, dwHeight);
				}
				lFindChannel = i;
			}
		}
	}
	m_mapChannelIsSet[lChannel] = lFindChannel;

	m_UniteLock.Unlock();
}


*/