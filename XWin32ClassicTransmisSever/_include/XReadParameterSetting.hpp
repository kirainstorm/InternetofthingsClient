#ifndef x_read_param_setting_kmkzcwe3rxc76q34ghsdckwrbh943
#define x_read_param_setting_kmkzcwe3rxc76q34ghsdckwrbh943

//XInternet.ini
/*
[License]
License=9888486A5C0F7F021F97B0CEDD7377504F4364748BC7A21500EA8E6E5B542C756581484E3686190A7AD
[Classic]
Serial=SERVC
DatabaseCenter=DSN=ClassicPlatCenter;Uid=sa;Pwd=Vmrs1234
DatabaseLocal=DSN=DBPlatCenter;Uid=sa;Pwd=Vmrs1234
[Iot]
Serial=32.3.4.1
DatabaseCenter=DSN=IOTDatabaseCenter;Uid=rshks@rshks;Pwd=Vmrs1234
DatabaseLocal=DSN=IOTDatabaseLocal;UUid=sa;Pwd=Vmrs1234
[Store]
Database=DSN=StoreDatabase;Uid=sa;Pwd=Vmrs1234
Table=hislog
Disk=c
[Rtmp]
IP=47.96.249.142
[SSL]
CertPem=c:/214539308040499.pem
Key=c:/214539308040499.key
Pwd=214539308040499
*/

class CXReadParameterSetting
{
public:
	CXReadParameterSetting(void) {
		read();
	};
	~CXReadParameterSetting(void) {
	};
public:
	static CXReadParameterSetting& Instance() {
		static CXReadParameterSetting ins;
		return ins;
	};

public:
	string strLicense;
	//
	string strClassicSerial, strClassicDatabaseCenter, strClassicDatabaseLocal;
	//
	string strIotSerial, strIotDatabaseCenter, strIotDatabaseLocal;
	//
	string strStoreDatabase, strStoreTable, strStoreDisk;
	//
	string strRtmpServerIP;
	//
	string strSslCertFilePem, strSslKeyFileKey, strSslPwd;
private:
	void read()
	{
		char strModulePath[MAX_PATH] = { 0 };
		::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
		std::string inifile(strModulePath);
		size_t nCount = inifile.find_last_of("\\");
		if (nCount < 0)
			nCount = inifile.find_last_of("/");
		if (nCount >= 0)
			inifile = inifile.substr(0, nCount + 1);
		inifile.append("\\XInternet.ini");
		//
		char a[512] = { 0 };
		//
		//=================================================================================================================
		//=================================================================================================================
		//=================================================================================================================
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("License", "License", "", a, sizeof(a), inifile.c_str());
		strLicense = a;
		//=================================================================================================================
		//=================================================================================================================
		//=================================================================================================================
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Classic", "Serial", "", a, sizeof(a), inifile.c_str());
		strClassicSerial = a;
		//
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Classic", "DatabaseCenter", "", a, sizeof(a), inifile.c_str());
		strClassicDatabaseCenter = a;
		//
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Classic", "DatabaseLocal", "", a, sizeof(a), inifile.c_str());
		strIotDatabaseLocal = a;
		//=================================================================================================================
		//=================================================================================================================
		//=================================================================================================================
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Iot", "Serial", "", a, sizeof(a), inifile.c_str());
		strIotSerial = a;
		//
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Iot", "DatabaseCenter", "", a, sizeof(a), inifile.c_str());
		strIotDatabaseCenter = a;
		//
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Iot", "DatabaseLocal", "", a, sizeof(a), inifile.c_str());
		strIotDatabaseLocal = a;
		//=================================================================================================================
		//=================================================================================================================
		//=================================================================================================================
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Store", "Database", "", a, sizeof(a), inifile.c_str());
		strStoreDatabase = a;
		//
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Store", "Table", "", a, sizeof(a), inifile.c_str());
		strStoreTable = a;
		//
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Store", "Disk", "", a, sizeof(a), inifile.c_str());
		strStoreDisk = a;
		//=================================================================================================================
		//=================================================================================================================
		//=================================================================================================================
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("Rtmp", "IP", "", a, sizeof(a), inifile.c_str());
		strRtmpServerIP = a;
		//=================================================================================================================
		//=================================================================================================================
		//=================================================================================================================
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("SSL", "CertPem", "", a, sizeof(a), inifile.c_str());
		strSslCertFilePem = a;
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("SSL", "Key", "", a, sizeof(a), inifile.c_str());
		strSslKeyFileKey = a;
		memset(a, 0, sizeof(a));
		::GetPrivateProfileStringA("SSL", "Pwd", "", a, sizeof(a), inifile.c_str());
		strSslPwd = a;

	}
};

#endif