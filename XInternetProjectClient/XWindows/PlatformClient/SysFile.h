

#ifndef SYS_FILE_SET_GET_EWR23KJ4H89MN
#define SYS_FILE_SET_GET_EWR23KJ4H89MN

#include <stdio.h>

struct Ssy_Config_Struct
{
	//
	long lg;//language 0��ZH_CN  1:english 

	//
	char loginuser[64];
	char loginpwd[64];
	char serverip[64];

	//
	char picPath[256];//ͼƬ���·��//��ѡ����ļ������Զ�����PicturesĿ¼
	char recPath[256];//¼����·��
	//����¼��recPath[256]\\Mp4Files\\%d
	//����¼��recPath[256]\\Mp4Files\\%d\\Download

	//
	int spiltScreen;//��Ļ�ָ���,1;4;6;8;9;16;25;

	int enablpwd;//�Ƿ��ס����
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
		fopen_s(&file, m_szFileName, "r");//r ��
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
		fopen_s(&file, m_szFileName, "wb");//wb ֻд�򿪻��½�һ���������ļ������ļ��������ļ�������Ϊ0��ֻ����д���ݡ�
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
r ��ֻ����ʽ���ļ������ļ�������ڡ�

r+ �Կɶ�д��ʽ���ļ������ļ�������ڡ�

rb+ ��д��һ���������ļ���ֻ�����д���ݡ�

rt+ ��д��һ���ı��ļ����������д��

w ��ֻд�ļ������ļ��������ļ�������Ϊ0�������ļ����ݻ���ʧ�����ļ��������������ļ���

w+ �򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������������ļ���

a �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�����ļ�ԭ�ȵ����ݻᱻ��������EOF��������

a+ �Ը��ӷ�ʽ�򿪿ɶ�д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������ ��ԭ����EOF����������

wb ֻд�򿪻��½�һ���������ļ���ֻ����д���ݡ�

wb+ ��д�򿪻���һ���������ļ����������д��

wt+ ��д�򿪻��Ž���һ���ı��ļ��������д��

at+ ��д��һ���ı��ļ�������������ı�ĩ׷�����ݡ�

ab+ ��д��һ���������ļ�������������ļ�ĩ׷�����ݡ�

*/



#endif//SYS_FILE_SET_GET_EWR23KJ4H89MN