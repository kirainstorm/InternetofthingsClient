#ifndef OPENGL_RENDERER_H_RTHJLNCDFGHJKAMJLAKLIIYGBMN
#define OPENGL_RENDERER_H_RTHJLNCDFGHJKAMJLAKLIIYGBMN

#include "PlatformDefine.h"
//#include "DecFrame.h"


#ifdef _PLATFORM_ANDROID


//#include "XCriticalSection.h"
//#include "XCross.h"
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
	char * m_buffer;//δ��������
	int m_bufferlen;

public:
	//char * m_frame;//���������
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
#ifdef _PLATFORM_ANDROID

#include <jni.h>
#include <android/log.h>

//#include <GLES2/glext2.h>
#ifdef _PLATFORM_ANDROID
//#include "FFmpegDecoder.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_G_BUF_SIZE_SIZE (2*1920*1080)

const GLchar fragmentShaderCode[] = "uniform sampler2D Ytex;\n\
											uniform sampler2D Utex;\n\
													uniform sampler2D Vtex;\n\
															precision mediump float;  \n\
																	varying vec4 VaryingTexCoord0; \n\
																			vec4 color;\n\
																					void main()\n\
																							{\n\
																									float yuv0 = (texture2D(Ytex,VaryingTexCoord0.xy)).r;\n\
																											float yuv1 = (texture2D(Utex,VaryingTexCoord0.xy)).r;\n\
																													float yuv2 = (texture2D(Vtex,VaryingTexCoord0.xy)).r;\n\
																															\n\
																																	color.r = yuv0 + 1.4022 * yuv2 - 0.7011;\n\
																																			color.r = (color.r < 0.0) ? 0.0 : ((color.r > 1.0) ? 1.0 : color.r);\n\
																																					color.g = yuv0 - 0.3456 * yuv1 - 0.7145 * yuv2 + 0.53005;\n\
																																							color.g = (color.g < 0.0) ? 0.0 : ((color.g > 1.0) ? 1.0 : color.g);\n\
																																									color.b = yuv0 + 1.771 * yuv1 - 0.8855;\n\
																																											color.b = (color.b < 0.0) ? 0.0 : ((color.b > 1.0) ? 1.0 : color.b);\n\
																																													gl_FragColor = color;\n\
																																															}\n";

const GLchar vertexShaderCode[] = "uniform mat4 uMVPMatrix;   \n\
								  		attribute vec4 vPosition;  \n\
												attribute vec4 myTexCoord; \n\
														varying vec4 VaryingTexCoord0; \n\
																void main(){               \n\
																		VaryingTexCoord0 = myTexCoord; \n\
																				gl_Position = vPosition; \n\
																						}  \n";

const GLfloat textCoodBufferData[] = {
	0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f };

const GLfloat positionBufferData[] = {
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f };


static void checkGlError(const char* op)
{
	GLint error;
	for (error = glGetError(); error; error = glGetError())
	{
		LOGD("error::after %s() glError (0x%x)\n", op, error);
	}
}

COpenglRenderer::COpenglRenderer() {
	LOGD(">>>>>>>COpenglRenderer::COpenglRenderer()");

	programHandle = 0;
	fragmentShader = 0;
	vertexShader = 0;
	g_buffer = (char *)malloc(MAX_G_BUF_SIZE_SIZE);


	struct tm *p;
	time_t t;

	time(&t);
	p = localtime(&t);

	year = 1900 + p->tm_year;
	month = 1 + p->tm_mon;


}

COpenglRenderer::~COpenglRenderer() {
	destroyProgram();
	if (g_buffer)
		free(g_buffer);
}

void COpenglRenderer::init() {
	LOGD(">>>>>>>opengl init");
	m_bExit = FALSE;
	m_bPause = FALSE;
	m_bClear = FALSE;
	mWidth = 0;
	mHeight = 0;
	//m_bFirst = 0;
	// Set the background color to black ( rgba )
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

#if 1
	destroyProgram();

	buildProgram(vertexShaderCode, fragmentShaderCode);
	glUseProgram(programHandle);
	glGenTextures(1, &g_texYId);
	glGenTextures(1, &g_texUId);
	glGenTextures(1, &g_texVId);
#endif



#ifdef FREAME_COUNT_TEST
	m_count = 0;
	m_aa11 = 0;
	gettimeofday(&t_start, NULL);
	start = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
#endif

	while (m_frameList.size()>0)
	{
		m_frameList.pop_front();
	}
}
void COpenglRenderer::Viewport(int x, int y, int width, int height) {

	if (m_bExit)
	{
		return;
	}

	LOGD(">>>>>>>opengl resize: %d %d %d %d", x, y, width, height);
	//glDisable(GL_DITHER); // ��ɫ������˵��������Ӱ�����ܣ����� 
	glViewport(x, y, width, height);
}
void COpenglRenderer::Pause()
{
	m_bPause = TRUE;
}
void COpenglRenderer::Resume()
{
	m_mutex.Lock();
	while (m_frameList.size() > 0)
	{
		m_frameList.pop_front();
	}
	m_mutex.Unlock();
	m_bClear = FALSE;
	m_bPause = FALSE;
}
void COpenglRenderer::clear()
{
	mWidth = 0;
	m_bClear = TRUE;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
}
//������С��ϵ���ֻ���step�ĵ���Ƶ��15~20msһ��
//sony st25i ����MX ����0ms
void COpenglRenderer::step() {


	//--------------------------------------------------------
	//gettimeofday(&t_start, NULL);
	//long end = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
	//--------------------------------------------------------

	// 	gettimeofday(&t_start, NULL);
	// 	long m = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
	// 	int k = 40 - (m - start);
	// 
	// 	LOGD(">>>>>>>step: sleep time=%d", 40 - k);
	// 
	// 	gettimeofday(&t_start, NULL);
	// 	start = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
	// 	return;

	if (m_bExit)
	{
		return;
	}

	if (m_bClear)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
		return;
	}


	if (m_frameList.size() <= 0)
	{
		if (mWidth != 0)
			draw();
		else
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
		}
		return;
	}

	//����������ر���Զ��
	m_cs.Lock();

	// 	if (m_bExit)
	// 	{
	// 		return;
	// 	}

	//usleep(33*1000);

	gettimeofday(&t_start, NULL);
	long end = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
	int n = 33 - (end - start);
	n = (n < 0) ? 0 : n;
	// 
	// 

	//LOGD(">>>>>>>step:mmmmmmmmmmmmmm   %d",n);

	if (n > 3)
	{
		//if (g_buffer)
		//draw();
		// 		if (m_bFirst == 0)
		// 		{
		// 			m_bFirst = 1;
		// 		}
		// 		else
		// 		{
		usleep(n * 1000);
		//		}

		//m_cs.Unlock();
		//return;
	}

	// 	while ((m_frameList.size() <= 0))
	// 	{
	// 		if (m_bExit)
	// 		{
	// 			return;
	// 		}
	// 		usleep(1000);
	// 	}
	// 
	// 	if (m_frameList.size() <= 0)
	// 	{
	// 		//glClear(GL_COLOR_BUFFER_BIT);
	// 		if (g_buffer)
	// 			draw();
	// 		//usleep(1000);
	// 		m_cs.Unlock();
	// 		return;
	// 	}

	//gettimeofday(&t_start, NULL);
	//end = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
	//n = 37 - (end - start);
	//LOGD(">>>>>>>step: sleep time=%d", int(end - start));

	// 	if (n)
	// 	{
	// 		LOGD(">>>>>>>step: sleep time=%d", int(n));
	// 		usleep(n);
	// 	}

	//usleep(1000);
	//LOGD(">>>>>>>opengl step 111");
	//if (mWidth == 0)
	//	return;

	//

	gettimeofday(&t_start, NULL);
	start = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;


	// 	CDecFrame * p = m_frameList.front().get();
	// 	char *pYUV = p->GetFrame();
	// 
	// 	if (pYUV == NULL)
	// 	{
	// 		m_mutex.Lock();
	// 		m_frameList.pop_front();
	// 		m_mutex.Unlock();
	// 		m_cs.Unlock();
	// 		return;
	// 	}





	//glClear(16384);


	//LOGD(">>>>>>>opengl step 222");

	m_mutexDraw.Lock();


	if (programHandle != 0)
	{
		//LOGD(">>>>>>>opengl step 333");
		//m_mutex.Lock();



		COpenglRenderFrame * p = m_frameList.front().get();
		char *pYUV = p->m_buffer;



		mWidth = p->m_width;
		mHeight = p->m_height;

		//pY = pYUV;
		//pU = pYUV + mWidth * mHeight;
		//pV = pYUV + 5 * (mWidth * mHeight) / 4;


		//if (g_width != width || g_height != height)
		//{
		int buffersize = 6 * (mWidth * mHeight) / 4;


		//g_width = width;
		//g_height = height;

		//g_buffer = (char *)malloc(buffersize);
		//}

		memset(g_buffer, 0, buffersize + 1);
		memcpy(g_buffer, pYUV, buffersize);




		// 		m_mutex.Lock();
		// 
		// 		memset(pY, 0, sizeof(pY));
		// 		memset(pU, 0, sizeof(pU));
		// 		memset(pV, 0, sizeof(pV));
		// 
		// 
		// 		memcpy(pY, yuvbuf, width * height);
		// 		memcpy(pU, yuvbuf + width * height, width * height / 4);
		// 		memcpy(pV, yuvbuf + 5 * (width * height) / 4, width * height / 4);
		// 
		// 		mHeight = height;
		// 		mWidth = width;
		// 
		// 		m_mutex.Unlock();

		draw();
#ifdef FREAME_COUNT_TEST
		gettimeofday(&t_start, NULL);
		long end = ((long)t_start.tv_sec) * 1000 + (long)t_start.tv_usec / 1000;
		int cost_time = (end - start);

		//LOGD(">>>>>>>step: %d  draw time=%d", m_aa11++, cost_time);
#endif
		m_mutex.Lock();
		m_frameList.pop_front();
		m_mutex.Unlock();
	}
	m_mutexDraw.Unlock();


	m_cs.Unlock();


}



int COpenglRenderer::writeSample(const char* pDecData, int nLen, int nWidth, int nHeight)
{
	//return 0;

	// 	GetCurTime(m_pdatetime);
	// 	if (m_pdatetime->year > 2015)
	// 	{
	// 		return -1;
	// 	}
	// 	else if (m_pdatetime->month > 8)
	// 	{
	// 		return -1;
	// 	}
	if (m_bPause)
	{
		//LOGD(">>>>>>>writeSample  m_bPause!!!!!!");
		return 0;
	}
	if (m_bClear)
	{
		//LOGD(">>>>>>>writeSample  m_bPause!!!!!!");
		return 0;
	}

#ifdef FREAME_COUNT_TEST
	//LOGD(">>>>>>>writeSample: %d   COpenglRenderer::m_frameList.size()=%d year=%d month=%d",
	//m_count++, m_frameList.size(), year, month);
#endif

	// 	if (year > 2015)
	// 	{
	// 	return -1;
	// 	}
	// 	else if (month > 8)
	// 	{
	// 	return -1;
	// 	}



	boost::shared_ptr<COpenglRenderFrame> pFrame(new COpenglRenderFrame(nLen));
	COpenglRenderFrame *p = pFrame.get();

	memcpy(p->m_buffer, pDecData, nLen);
	p->m_bufferlen = nLen;
	p->m_width = nWidth;
	p->m_height = nHeight;


	m_mutex.Lock();
	m_frameList.push_back(pFrame);
	m_mutex.Unlock();

	return 0;
}

///////////////////////////////////////////////////
GLuint COpenglRenderer::buildShader(const char* source, GLenum shaderType)
{
	GLuint shaderHandle = glCreateShader(shaderType);
	checkGlError("glCreateShader");

	if (shaderHandle)
	{
		glShaderSource(shaderHandle, 1, &source, 0);
		checkGlError("glShaderSource");
		glCompileShader(shaderHandle);
		checkGlError("glCompileShader");

		GLint compiled = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				char* buf = (char*)malloc(infoLen);
				if (buf)
				{
					glGetShaderInfoLog(shaderHandle, infoLen, NULL, buf);
					LOGD("error::Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}
				glDeleteShader(shaderHandle);
				shaderHandle = 0;
			}
		}

	}

	return shaderHandle;
}

GLuint COpenglRenderer::buildProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	vertexShader = buildShader(vertexShaderSource, GL_VERTEX_SHADER);
	fragmentShader = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	programHandle = glCreateProgram();

	if (programHandle)
	{
		glAttachShader(programHandle, vertexShader);
		checkGlError("glAttachShader vertexShader");
		glAttachShader(programHandle, fragmentShader);
		checkGlError("glAttachShader fragmentShader");
		glLinkProgram(programHandle);

		GLint linkStatus = GL_FALSE;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*)malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(programHandle, bufLength, NULL, buf);
					LOGD("error::Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(programHandle);
			programHandle = 0;
		}

	}

	if (programHandle)
	{
		tex_y = glGetUniformLocation(programHandle, "Ytex");
		checkGlError("glGetUniformLocation");
		tex_u = glGetUniformLocation(programHandle, "Utex");
		checkGlError("glGetUniformLocation");
		tex_v = glGetUniformLocation(programHandle, "Vtex");
		checkGlError("glGetUniformLocation");


		//glBindAttribLocation(simpleProgram, ATTRIB_VERTEX, "vPosition");
		ATTRIB_VERTEX = glGetAttribLocation(programHandle, "vPosition");
		checkGlError("glBindAttribLocation");
		//glBindAttribLocation(simpleProgram, ATTRIB_TEXTURE, "a_texCoord");
		ATTRIB_TEXTURE = glGetAttribLocation(programHandle, "myTexCoord");
		checkGlError("glBindAttribLocation");
	}

	return programHandle;
}
GLuint COpenglRenderer::destroyProgram()
{
#if 0
	m_mutexDraw.Lock();



	if (programHandle != 0) {
		glDetachShader(programHandle, fragmentShader);
		checkGlError("glDetachShader fragmentShader");
		glDetachShader(programHandle, vertexShader);
		checkGlError("glDetachShader vertexShader");
		glDeleteProgram(programHandle);
		checkGlError("glDeleteProgram");
		programHandle = 0;
	}
	if (fragmentShader != 0) {
		glDeleteShader(fragmentShader);
		checkGlError("glDeleteShader fragmentShader");
		fragmentShader = 0;
	}
	if (vertexShader != 0) {
		glDeleteShader(vertexShader);
		checkGlError("glDeleteShader vertexShader");
		vertexShader = 0;
	}

	//if (g_buffer)
	//	free(g_buffer);
	m_mutexDraw.Unlock();
#endif

	return 0L;
}

// long COpenglRenderer::destroyShaders() {
// 	//unloadVBOs();
// 
// }

///////////////////////////////////////////////////
int COpenglRenderer::BindTexture(int texture, int mWidth, int mHeight, char * paramBuffer) {
	/*
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mWidth, mHeight, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, paramBuffer);
	*/

	checkGlError("glGenTextures");
	glBindTexture(GL_TEXTURE_2D, texture);
	checkGlError("glBindTexture");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mWidth, mHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, paramBuffer);
	checkGlError("glTexImage2D");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkGlError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGlError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	checkGlError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	checkGlError("glTexParameteri");

	return 0;
}

int COpenglRenderer::draw() {
	//LOGD(">>>>>>>opengl draw");

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);


	//glClearColor(0.5f, 0.5f, 0.5f, 1);
	//checkGlError("glClearColor");
	//glClear(GL_COLOR_BUFFER_BIT);
	//checkGlError("glClear");

	pY = g_buffer;
	pU = g_buffer + mWidth * mHeight;
	pV = g_buffer + 5 * (mWidth * mHeight) / 4;


	// Add program to OpenGL environment
	glUseProgram(programHandle);

	//y
	glActiveTexture(GL_TEXTURE0);
	BindTexture(g_texYId, mWidth, mHeight, pY);


	//u
	glActiveTexture(GL_TEXTURE1);
	BindTexture(g_texUId, mWidth >> 1, mHeight >> 1, pU);


	//v
	glActiveTexture(GL_TEXTURE2);
	BindTexture(g_texVId, mWidth >> 1, mHeight >> 1, pV);

	glUniform1i(tex_y, 0);
	glUniform1i(tex_u, 1);
	glUniform1i(tex_v, 2);


	// Enable a handle to the triangle vertices
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 4, GL_FLOAT, false, 0, positionBufferData);

	// Enable a handle to the triangle vertices
	glEnableVertexAttribArray(ATTRIB_TEXTURE);
	glVertexAttribPointer(ATTRIB_TEXTURE, 4, GL_FLOAT, false, 0, textCoodBufferData);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_TEXTURE);


	//mWidth = 0;
	return 0;
}

#endif
#endif
