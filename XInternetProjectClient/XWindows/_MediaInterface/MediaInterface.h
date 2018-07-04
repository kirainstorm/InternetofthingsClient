#ifndef MEDIA_INTERFACE_JHIU3JOI3KJJKSND_2352KLUHJ_3
#define MEDIA_INTERFACE_JHIU3JOI3KJJKSND_2352KLUHJ_3

#include "XClientStructDefine.h"
#ifdef __cplusplus
extern "C" {
#endif

	void XMediaInferfaceInit();
	void XMediaInferfaceUninit();

	CXClientInferface * XMediaCreateClientInferface(emXClientType type);



#ifdef __cplusplus
}
#endif

#endif