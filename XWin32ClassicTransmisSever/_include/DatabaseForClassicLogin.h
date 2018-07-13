#pragma once
//
//------------------------------------------------------------------------
// 用于经典模式服务器的设备或者用户登陆校验
//------------------------------------------------------------------------
//
#include "PlatformDefine.h"
#include "XReadParameterSetting.hpp"
#include "XCounter.hpp"
//
class CDBForClassicLogin
{
public:
	CDBForClassicLogin()
	{
		m_hWorkerThread = CROSS_THREAD_NULL;
		m_hEvtWorkerStop = FALSE;
		m_bSqlError = TRUE;
	};
	~CDBForClassicLogin()
	{
		try {
			sql.close();
		}
		catch (...)
		{
		}
	};

public:
	static CDBForClassicLogin& Instance()
	{
		static CDBForClassicLogin inc;
		return inc;
	};
	void Start()
	{
#if THIS_CLASSIC_DB_LOGIN_FOR_MESSAGE_SERVER

#else
		//经典服务器在流转发服务器中更新设备在线状态
		if (m_hWorkerThread == CROSS_THREAD_NULL)
		{
			m_hEvtWorkerStop = FALSE;
			m_hWorkerThread = CrossCreateThread(WorkerThread, this);
		}
#endif
	};
	void Stop()//放在CRecycleService::Instance().Stop()之后
	{
#if THIS_CLASSIC_DB_LOGIN_FOR_MESSAGE_SERVER

#else
		//经典服务器在流转发服务器中更新设备在线状态
		if (!m_hEvtWorkerStop)
		{
			m_hEvtWorkerStop = TRUE;
			CrossWaitThread(m_hWorkerThread);
			CrossCloseThread(m_hWorkerThread);
			m_hWorkerThread = CROSS_THREAD_NULL;
		}
#endif
	}
	void OpenSql()
	{
		CXCounter::Instance().AddDatabaseClassic();
		CloseSql();
		//m_cs.Lock();
		try {
			//sql.close();//关闭已有连接，如果已经调用了sql.open再调用sql.open无效
			soci::connection_parameters parameters(soci::odbc, CXReadParameterSetting::Instance().strClassicDatabaseCenter);
			parameters.set_option(soci::odbc_option_driver_complete, "0" /* SQL_DRIVER_NOPROMPT */);
			sql.open(parameters);
			m_bSqlError = FALSE;
		}
		catch (soci::odbc_soci_error const & e)
		{
			m_bSqlError = TRUE;
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
		}
		catch (exception const &e)
		{
			m_bSqlError = TRUE;
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
		}
		catch (...)
		{
			m_bSqlError = TRUE;
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
		}
		//m_cs.Unlock();
	};
	void CloseSql()
	{
		try
		{
			m_bSqlError = TRUE;
			sql.close();
		}
		catch (soci::odbc_soci_error const & e)
		{
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
		}
		catch (exception const &e)
		{
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
		}
		catch (...)
		{
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
		}
		//
		m_bSqlError = TRUE;
	};
	BOOL IsSqlStatusError()
	{
		return m_bSqlError; //!!!!!
	};
public:
	BOOL DeviceLoginServer(uint32_t _serial, int &_dbid, int &_mainsub)
	{
		BOOL bCanLogin = FALSE;
		if (m_bSqlError)
		{
			return FALSE;//立刻返回,减少对锁的占用
		}
		m_cs.Lock();
		do
		{
			if (m_bSqlError)
			{
				break;//立刻返回
			}

			//------------------------------------------------------------------------------
			try {
				string s = "SELECT d.ID, d.D_Stream_Main_Sub  FROM [Devices] as d "\
					"INNER JOIN Servers as s ON s.ID=d.CoreServerID "\
					" where s.S_Serial = :S_Serial and D_Serial=:D_Serial ";
				soci::rowset<soci::row> rs = (sql.prepare << s, soci::use(CXReadParameterSetting::Instance().strClassicSerial), soci::use(_serial));
				soci::rowset<soci::row>::const_iterator it = rs.begin();
				if (it != rs.end())
				{
					soci::row const& row = *it;
					_dbid = row.get<int>(0);
					_mainsub = row.get<int>(1);
					bCanLogin = TRUE;
				}
			}
			catch (soci::odbc_soci_error const & e)
			{
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
				m_bSqlError = TRUE;
				break;
			}
			catch (exception const &e)
			{
				m_bSqlError = TRUE;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				break;
			}
			catch (...)
			{
				m_bSqlError = TRUE;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
				break;
			}
		} while (0);
		m_cs.Unlock();
		//
		return bCanLogin;
	};
	BOOL UserLoginStreamServer(char *username, char * userpwd)// , unsigned int devid)
	{
		//-------------------------------------------------------------------------------
		if ((0 == strcmp(username, "aTRns#torm@admin")) && (0 == strcmp(userpwd, "gsGTe#gjsd@ss")))
		{
			return TRUE;//CMediaAvDataInstance使用该账号登陆 [偷笑]
		}
		//-------------------------------------------------------------------------------
		BOOL bCanLogin = FALSE;
		if (m_bSqlError)
		{
			return bCanLogin;//立刻返回,减少对锁的占用
		}
		m_cs.Lock();
		do
		{
			if (m_bSqlError)
			{
				break;//立刻返回
			}

			//int dev_id = devid;
			string user = username;
			string pwd = userpwd;
			int count = 0;
			try {
#if 0
// 				string s = "select count(*) as cnt "\
// 					"FROM [Devices] as d "\
// 					"inner join [AccountGroupDevices] as agd on agd.DeviceID = d.ID "\
// 					"inner join [AccountGroups] as ag on agd.AcountGroupID = ag.ID "\
// 					"inner join [Accounts] as a on ag.ID = a.AcountGroupID "\
// 					"inner join [Servers] as s on s.ID = d.CoreServerID "\
// 					"where "\
// 					"d.ID = :ID and a.Ac_Phone = :Ac_Phone and a.Ac_Pwd = :Ac_Pwd";
// 				sql << s, soci::into(count), soci::use(dev_id), soci::use(user), soci::use(pwd);
#endif


				string s = "select count(*) as cnt from Accounts where Ac_Phone = :Ac_Phone and Ac_Pwd = :Ac_Pwd";
				sql << s, soci::into(count), soci::use(user), soci::use(pwd);

				bCanLogin = (count > 0) ? TRUE : FALSE;
			}
			catch (soci::odbc_soci_error const & e)
			{
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
				m_bSqlError = TRUE;
				break;
			}
			catch (exception const &e)
			{
				m_bSqlError = TRUE;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				break;
			}
			catch (...)
			{
				m_bSqlError = TRUE;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
				break;
			}

		} while (0);
		m_cs.Unlock();
		return bCanLogin;
	};

	BOOL UserLoginMessageServer(char *username, char * userpwd, vector<string> &devicelist , vector<string> &devicename, vector<int> &deviceonlie) //返回拥有的设备列表
	{
		BOOL bCanLogin = FALSE;

		if (m_bSqlError)
		{
			return bCanLogin;//立刻返回,减少对锁的占用
		}

		m_cs.Lock();
		do
		{
			if (m_bSqlError)
			{
				break;//立刻返回
			}

			string user = username;
			string pwd = userpwd;

			try {
				string str = "select Ac_Phone "\
					"FROM [Accounts] "\
					"where Ac_Phone = :Ac_Phone and Ac_Pwd = :Ac_Pwd ";

				soci::rowset<soci::row> rs_user = (sql.prepare << str, soci::use(user), soci::use(pwd));
				if (rs_user.begin() == rs_user.end())
				{
					break;
				}

				str = "select d.ID, D.D_Name, D.D_Is_Online as cnt "\
					"FROM [Devices] as d "\
					"inner join [AccountGroupDevices] as agd on agd.DeviceID = d.ID "\
					"inner join [AccountGroups] as ag on agd.AcountGroupID = ag.ID "\
					"inner join [Accounts] as a on ag.ID = a.AcountGroupID "\
					"inner join [Servers] as s on s.ID = d.CoreServerID "\
					"where "\
					"a.Ac_Phone = :Ac_Phone and a.Ac_Pwd = :Ac_Pwd order by D.D_Is_Online desc , d.ID asc";

				soci::rowset<soci::row> rs = (sql.prepare << str, soci::use(user), soci::use(pwd));

				//这样写不行，少一条记录，why？难道是迭代器第二次使用的时候又迭代了一次？ //以后查原因 2018/03/19 start
				//soci::rowset<soci::row>::const_iterator it = rs.begin();
				//for (it = rs.begin(); it != rs.end(); ++it)
				//应该是(it; it != rs.end(); ++it)
				// 2018/03/19 end
				for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
				{
					soci::row const& row = *it;
					int _dbid = row.get<int>(0);
					string strid = to_string(_dbid);

					//
					devicelist.push_back(strid);
					devicename.push_back(AsciiToUtf8(row.get<string>(1)));
					deviceonlie.push_back(row.get<int>(2));
				}
				bCanLogin = TRUE;
			}
			catch (soci::odbc_soci_error const & e)
			{
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
				m_bSqlError = TRUE;
				break;
			}
			catch (exception const &e)
			{
				m_bSqlError = TRUE;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				break;
			}
			catch (...)
			{
				m_bSqlError = TRUE;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
				break;
			}

		} while (0);
		m_cs.Unlock();
		return bCanLogin;
	}
private:
#if THIS_CLASSIC_DB_LOGIN_FOR_MESSAGE_SERVER

#else
	static int	WorkerThread(void* param)
	{
		CDBForClassicLogin *pService = (CDBForClassicLogin*)param;
		pService->Worker();
		return 0;
	};
	void Worker()
	{
		//gServiceLogAddLog("CNetToServer::Login : login!!!!!!!!!!!!!!!!!!!!!!!!Worker m_hEvtWorkerStop=%d", m_hEvtWorkerStop);
		CROSS_DWORD64 dwUpdataDevices = CrossGetTickCount64();
		CROSS_DWORD64 dwUpdataDB = CrossGetTickCount64();
		//CROSS_DWORD64 dwStartLogTick = CrossGetTickCount64();
		DWORD nHour = 60 * 60 * 1000;
		do
		{

			//这里写设备在线状态到数据库

			vector<STDeviceOnlineStatus> r;
			CXStreamSubscriptionManagement::Instance().ClassicGetOnlineStatus(r);

			m_cs.Lock();

			if (!m_bSqlError)
			{

				try {
					sql.begin();
					sql << "update Devices set D_Browse_Num = 0 , D_Is_Online = 0";
					for (int i = 0; i < r.size(); i++)
					{
						int k = atoi(r[i].uuid.c_str());
						sql << "update Devices set D_Is_Online = :D_Is_Online where ID = :ID", soci::use(r[i].Online), soci::use(k);
					}
					sql.commit();
				}
				catch (soci::soci_error const & e)
				{
					Global_ServiceLog_AddLog("Get mediaServer devices failed! [%s]", e.what());
					m_bSqlError = TRUE;
					break;
				}
				catch (exception const &e)
				{
					m_bSqlError = TRUE;
					Global_ServiceLog_AddLog("xservice - db - catch (exception const &e) [%s]", e.what());
					break;
				}
				catch (...)
				{
					m_bSqlError = TRUE;
					Global_ServiceLog_AddLog("xservice - db - catch (...)");
					break;
				}
			}

			m_cs.Unlock();




			for (int i = 0; i < 30; i++)
			{
				Sleep(100);
				if (m_hEvtWorkerStop)
					break;
			}
		} while (!m_hEvtWorkerStop);


	};
#endif
private:
	std::string AsciiToUtf8(std::string str)
	{
#define MAX_LENGTH_ASCII_UTF8 4096
		WCHAR strSrc[MAX_LENGTH_ASCII_UTF8] = { 0 };
		char szRes[MAX_LENGTH_ASCII_UTF8] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, strSrc, MAX_LENGTH_ASCII_UTF8);
		WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, MAX_LENGTH_ASCII_UTF8, NULL, NULL);
		std::string result = szRes;
		return result;
	}

private:
	//
	volatile BOOL m_bSqlError;
	soci::session sql;
	std::string connectString;
	CrossCriticalSection m_cs;
	//
	volatile CROSS_THREAD_HANDLE	m_hWorkerThread;
	volatile BOOL m_hEvtWorkerStop;
	//
};