#pragma once
#include "PlatformDefine.h"
#include "XReadParameterSetting.hpp"

struct st_database_Transmis_stream_rtmp
{
	char url[1024];// 1:rtsp url a
};




// struct st_database_Transmis_stream_ssssrs
// {
// 	st_database_Transmis_stream t;
// 	char s[1024 - sizeof(st_database_Transmis_stream)];
// };
// inline st_database_Transmis_stream * malloc_atabase_Transmis_stream_info()
// {
// 	st_database_Transmis_stream_ssssrs * pBuffer = new st_database_Transmis_stream_ssssrs();
// 	assert(pBuffer);
// 	return  (st_database_Transmis_stream *)pBuffer;
// };
// inline void free_atabase_Transmis_stream_info(st_database_Transmis_stream *buffer)
// {
// 	st_database_Transmis_stream_ssssrs * pBuffer = (st_database_Transmis_stream_ssssrs *)buffer;
// 	delete pBuffer;
// };

//
class CDatabaseForClassicTransmis
{
public:
	CDatabaseForClassicTransmis()
	{
		m_bSqlError = TRUE;
	};
	~CDatabaseForClassicTransmis()
	{
		try {
			sql.close();
		}
		catch (...)
		{
		}
	};

public:
	static CDatabaseForClassicTransmis& Instance()
	{
		static CDatabaseForClassicTransmis inc;
		return inc;
	};
	void OpenSql()
	{
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
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
		}
		catch (exception const &e)
		{
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
		}
		catch (...)
		{
			//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
		}
		//
		m_bSqlError = TRUE;
	};
	BOOL IsSqlStatusError()
	{
		return m_bSqlError; //!!!!!
	};
public:
	void GetTransmisDevices(vector<st_database_Transmis_stream_rtmp *> & v)
	{

#if 0
		st_database_Transmis_stream_rtmp * p = new st_database_Transmis_stream_rtmp();
		CROSS_STRCPY(p->url, "rtsp://192.168.1.228:8554/stream1");
		v.push_back(p);

		return;
#endif



		if (m_bSqlError)
		{
			return;
		}
		m_cs.Lock();
		do
		{
			if (m_bSqlError)
			{
				break;
			}

			//------------------------------------------------------------------------------
			try {
#if 1
				string s = "SELECT d.D_Type , d.D_Content  FROM [Devices] as d "\
					"INNER JOIN Servers as s ON s.ID=d.CoreServerID "\
					" where s.S_Serial = :S_Serial";
				soci::rowset<soci::row> rs = (sql.prepare << s, soci::use(CXReadParameterSetting::Instance().strClassicSerial));
				soci::rowset<soci::row>::const_iterator it = rs.begin();
				if (it != rs.end())
				{
					soci::row const& row = *it;
					int _type = row.get<int>(0);
					string ip = row.get<string>(1);
					//
					st_database_Transmis_stream_rtmp * p = new st_database_Transmis_stream_rtmp();
					//
					CROSS_STRCPY(p->url, ip.c_str());
					//
					v.push_back(p);
				}
#else
				st_database_Transmis_stream_rtmp * p = new st_database_Transmis_stream_rtmp();
				CROSS_STRCPY(p->url, "rtsp://192.168.1.228:8554/stream1");
				v.push_back(p);
#endif

			}
			catch (soci::odbc_soci_error const & e)
			{
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, (const char *)e.odbc_error_message());
				m_bSqlError = TRUE;
				break;
			}
			catch (exception const &e)
			{
				m_bSqlError = TRUE;
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
				break;
			}
			catch (...)
			{
				m_bSqlError = TRUE;
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "catch...");
				break;
			}
		} while (0);
		m_cs.Unlock();
		//
	};
	

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
};