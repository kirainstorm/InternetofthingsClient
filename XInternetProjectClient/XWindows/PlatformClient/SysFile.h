

#ifndef SYS_FILE_SET_GET_EWR23KJ4H89MN
#define SYS_FILE_SET_GET_EWR23KJ4H89MN

#include <stdio.h>

struct Ssy_Config_Struct
{
	//
	long lg;//language 0：ZH_CN  1:english 

	//
	char loginuser[64];
	char loginpwd[64];
	char serverip[64];

	//
	char picPath[256];//图片存放路径//在选择的文件夹下自动生成Pictures目录
	char recPath[256];//录像存放路径
	//本地录像：recPath[256]\\Mp4Files\\%d
	//下载录像：recPath[256]\\Mp4Files\\%d\\Download

	//
	int spiltScreen;//屏幕分割数,1;4;6;8;9;16;25;

	int enablpwd;//是否记住密码
	//


	int loopplay_wnd;
	int loopplay_sec;
	int loopplaycount;
	char loopid[2048][32];
};


class CSysFileGetSet
{
public:
	CSysFileGetSet()
	{
		char strModulePath[MAX_PATH] = { 0 };
		::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
		std::string str(strModulePath);
		size_t nCount = str.find_last_of("\\");
		if (nCount < 0)
			nCount = str.find_last_of("/");

		if (nCount >= 0)
			str = str.substr(0, nCount + 1);

		str.append("\\M.TXT");

		memset(m_szFileName, 0, sizeof(m_szFileName));
		strcpy_s(m_szFileName, str.c_str());
	};
	~CSysFileGetSet()
	{

	};


public:
	static CSysFileGetSet& Instance()
	{
		static CSysFileGetSet inc;
		return inc;
	};
public:
	int Get(Ssy_Config_Struct *t)
	{
		FILE * file;
		fopen_s(&file, m_szFileName, "r");//r 读
		if (file == NULL)
		{
			strcpy_s(t->loginuser, "administrator");
			strcpy_s(t->loginpwd, "");
			strcpy_s(t->serverip, "127.0.0.1");
			std::string connectString;
			char strModulePath[MAX_PATH] = { 0 };
			::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
			std::string str(strModulePath);
			size_t nCount = str.find_last_of("\\");
			if (nCount < 0)
				nCount = str.find_last_of("/");
			if (nCount >= 0)
				str = str.substr(0, nCount + 1);
			strcpy_s(t->picPath, str.c_str());
			strcpy_s(t->recPath, str.c_str());
			t->spiltScreen = 4;
			t->lg = 0;
			t->spiltScreen = 4;
			t->enablpwd = 0;



			//
			t->loopplay_wnd = 4;
			t->loopplay_sec = 60;
			t->loopplaycount = 0;


			Set(*t);
			return 0;
		}

		fread(t, 1, sizeof(Ssy_Config_Struct), file);

		if (0 == t->loopplay_wnd)
		{
			t->loopplay_wnd = 4;
			t->loopplay_sec = 60;
			t->loopplaycount = 0;
		}
		fclose(file);
		return 0;
	};

	int Set(Ssy_Config_Struct t)
	{
		FILE * file;
		fopen_s(&file, m_szFileName, "wb");//wb 只写打开或新建一个二进制文件，若文件存在则文件长度清为0；只允许写数据。
		if (file == NULL)
		{
			return -1;
		}

		fwrite(&t, 1, sizeof(Ssy_Config_Struct), file);

		fclose(file);

		return 0;
	};

private:
	char m_szFileName[256];
};


/*
r 以只读方式打开文件，该文件必须存在。

r+ 以可读写方式打开文件，该文件必须存在。

rb+ 读写打开一个二进制文件，只允许读写数据。

rt+ 读写打开一个文本文件，允许读和写。

w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。

w+ 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。

a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）

a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）

wb 只写打开或新建一个二进制文件；只允许写数据。

wb+ 读写打开或建立一个二进制文件，允许读和写。

wt+ 读写打开或着建立一个文本文件；允许读写。

at+ 读写打开一个文本文件，允许读或在文本末追加数据。

ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。

*/



#endif//SYS_FILE_SET_GET_EWR23KJ4H89MN