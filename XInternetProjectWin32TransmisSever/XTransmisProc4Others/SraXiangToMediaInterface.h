#pragma once
class CSraXiangToMediaInterface :public XTransmisDeviceInterface
{
public:
	CSraXiangToMediaInterface();
	~CSraXiangToMediaInterface();

public:
	virtual void OpenTransmisDevice(char * p1 = NULL, char * p2 = NULL, char * p3 = NULL, char * p4 = NULL, int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0);
	virtual void CloseTransmisDevice();
	virtual BOOL GetError();



};

