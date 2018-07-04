#include "stdafx.h"
#include "DBForConfig.h"


#define backend_factory_center_connectString "DSN=DBPlatCenter;Uid=sa;Pwd=Vmrs1234"
soci::backend_factory const &backEndStore2PlatServer = *soci::factory_odbc();

CDBForConfig::CDBForConfig()
{
}

CDBForConfig::~CDBForConfig()
{
}

CDBForConfig& CDBForConfig::Instance()
{
	static CDBForConfig inc;
	return inc;
}

int CDBForConfig::GetAllDevices()
{
	int ret = -1;
	BOOL bOpenDB = FALSE;
	do 
	{
		m_stDevicves.clear();

		try {
			soci::connection_parameters parameters(backEndStore2PlatServer, backend_factory_center_connectString);
			parameters.set_option(soci::odbc_option_driver_complete, "0" /* SQL_DRIVER_NOPROMPT */);
			sql.open(parameters);
			bOpenDB = TRUE;
		}
		catch (soci::soci_error const & e)
		{
			CROSS_TRACE("sql.open failed! [%s]", e.what());
			break;
		}



		try {
			string s = "SELECT ID , D_Name FROM [Devices]";

			soci::rowset<soci::row> rs = (sql.prepare << s);
			for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
			{
				soci::row const& row = *it;
				ST_DEVICE_BASE st;
				memset(&st, 0, sizeof(st));


				st.dev_id = row.get<int>(0);
				string sss = row.get<string>(1);
				memcpy(st.dev_name, sss.c_str(), sss.length());
				st.time_start = 0;
				st.time_end = 0;

				m_stDevicves.push_back(st);
			}
		}
		catch (soci::soci_error const & e)
		{
			CROSS_TRACE("mediaServer devices failed! [%s]", e.what());
			break;
		}


		ret = 0;
	} while (0);


	if (bOpenDB)
	{
		sql.close();
	}
	return ret;
}