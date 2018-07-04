#ifndef __x_device_hpp_ghj65xcvb54sd98ik6r73433hbrv
#define __x_device_hpp_ghj65xcvb54sd98ik6r73433hbrv
#include "PlatformDefine.h"
#include "XDeviceInstance.h"
XDeviceInterface * CreateDeviceInstance()
{
	XDeviceInterface * p = new CXDeviceInstance();
	assert(p);
	return p;
};
#endif