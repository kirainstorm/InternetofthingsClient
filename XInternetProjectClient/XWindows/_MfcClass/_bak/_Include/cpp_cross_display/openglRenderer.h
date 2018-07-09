#ifndef _OPENGL_RENDERER_H_
#define _OPENGL_RENDERER_H_

#include "PlatformDefine.h"
//#include "DecFrame.h"


#ifdef _PLATFORM_ANDROID


//#include "XCriticalSection.h"
#include "XCross.h"
#include <GLES2/gl2.h>


//#define max_yuv_len (1024*1024*4)


#define  FREAME_COUNT_TEST


class COpenglRenderFrame
{
public:
	COpenglRenderFrame(int len){
		m_bufferlen = len;
		m_buffer = new char[len + 1];
		//memset(m_buffer, 0, len + 1);
	};
	~COpenglRenderFrame(){
		delete[] m_buffer;
	};
public:
	char * m_buffer;//未解码数据
	int m_bufferlen;

public:
	//char * m_frame;//解码后数据
	//int m_framelen;
	int m_width, m_height;
	//int key_frame;
};
// 
// #if 0
// class CFrame
// {
// public:
// 	CFrame(void)
// 	{
// 		m_width = 0;
// 		m_height = 0;
// 		m_frame = NULL;
// 	};
// 	~CFrame(void)
// 	{
// 		if (m_frame)
// 		{
// 			delete[] m_frame;
// 		}
// 	};
// 
// public:
// 	void SetFrame(char* pStreamData, int width, int height)
// 	{
// 		int lLen = width*height * 6 / 4;
// 
// 		if (lLen < 0)
// 		{
// 			return;
// 		}
// 		if (lLen > max_yuv_len)
// 		{
// 			return;
// 		}
// 
// 
// 		m_frame = new char[lLen + 1];
// 		memset(m_frame, 0, sizeof(m_frame));
// 		memcpy(m_frame, pStreamData, lLen);
// 		m_width = width;
// 		m_height = height;
// 
// 	};
// 	int GetWidth()
// 	{
// 		return m_width;
// 	};
// 	int GetHeight()
// 	{
// 		return m_height;
// 	};
// 	char * GetFrame()
// 	{
// 		return m_frame;
// 	};
// 
// private:
// 	char * m_frame;
// 	int m_width, m_height;
// 
// };
// #endif




class COpenglRenderer
{
public:
	COpenglRenderer();
	~COpenglRenderer();



#ifdef FREAME_COUNT_TEST
	struct timeval t_start;
	int m_count, m_aa11;
	long start;
	int year, month;
#endif

	void init();
	void step();
	void Viewport(int x ,int y, int width, int height);
	void destroy(){ m_bExit = TRUE; destroyProgram(); };
	void Pause();
	void Resume();
	void clear();


	int writeSample(const char* pDecData, int nLen, int nWidth, int nHeight);

private:
	CrossCriticalSection m_mutex;
	char * pY, *pU, *pV;//yuv data
	int mHeight, mWidth;//video width height

	BOOL m_bPause;
	BOOL m_bClear;
private:
	CrossCriticalSection m_mutexDraw;
	//GLuint texture[3];

	int vertexShader;
	int fragmentShader;
	int programHandle;

	GLuint g_texYId;
	GLuint g_texUId;
	GLuint g_texVId;

	int ATTRIB_VERTEX, ATTRIB_TEXTURE;
	GLint tex_y, tex_u, tex_v;
	char *              g_buffer;

	int BindTexture(int texture, int mWidth, int mHeight, char * paramBuffer);
	int draw();//

private:
	GLuint buildShader(const char* source, GLenum shaderType);
	GLuint buildProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	GLuint destroyProgram();


	//int compileShader(const GLchar * shaderCode, int type);
	//long createShaders();
	//long destroyShaders();




	//int textureSlot[3];

	//static GLuint g_texYId;
	//static GLuint g_texUId;
	//static GLuint g_texVId;
	//static GLuint simpleProgram;


private:
	list< boost::shared_ptr<COpenglRenderFrame> > m_frameList;

	int m_bFirst;
	CrossCriticalSection m_cs;


	mutable BOOL m_bExit;
};


#endif

#endif
