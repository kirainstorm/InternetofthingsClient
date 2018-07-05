#include "Encode264.h"
#include "PhoneDevice.h"


void * CreateEncode264Instance(int width, int height,int a_chanel, int a_sample_rate, int a_bit_rate)
{
	CPhoneDevice * p = new CPhoneDevice( a_chanel,  a_sample_rate,  a_bit_rate);
	p->Start( width,  height);
	return p;
}
void InputData(void * ins, int enc264type, char * buffer, int len)
{
	CPhoneDevice * p = (CPhoneDevice *)ins;
	p->InputData(enc264type,buffer, len);
}
void DestroyEncode264Instance(void *ins)
{
	CPhoneDevice * p = (CPhoneDevice *)ins;
	p->Stop();
	delete p;
}
