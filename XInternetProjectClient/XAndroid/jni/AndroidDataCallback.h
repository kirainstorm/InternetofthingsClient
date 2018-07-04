#ifndef android_data_cb_welk234jio23RDFREEW
#define android_data_cb_welk234jio23RDFREEW

#include "XPlayerOpenGLRenderer.hpp"
#include "XPlayer.hpp"


class CAndroidDataCallback :public DecodeVideoCallBack
{
public:
	CAndroidDataCallback(COpenglRenderer *render){
	m_render = render;
};
	~CAndroidDataCallback(){};
public:
	virtual int OnDecodeVideoCallBack(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0)
	{
	CROSS_TRACE("CAndroidDataCallback::OnDecodeVideoCallBack ------------------------- %d %d %d", nLen, nWidth, nHeight);

	if (m_render)
	{
		//CROSS_TRACE("CAndroidDataCallback::OnDecodeVideoCallBack ------------------------- 2");
		m_render->writeSample(pDecData, nLen, nWidth, nHeight);
	}
	return 0;
};
private:
	COpenglRenderer *m_render;
};
#endif

