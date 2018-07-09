#pragma once

//������������ͳ��



class CXTestCounter
{
public:
	CXTestCounter() {};
	~CXTestCounter() {};
	//
	static CXTestCounter& Instance()
	{
		static CXTestCounter inc;
		return inc;
	};
	//
	void Reset(uint32_t count)
	{
		m_nAll = count;
		m_nSuccess = 0;
		m_nFailed = count;
	};
	//
	void AddSuccess()//�ɹ�����
	{
		m_cs.Lock();
		++m_nSuccess;
		m_cs.Unlock();
	};
	void ReleaseSuccess()
	{
		m_cs.Lock();
		--m_nSuccess;
		m_cs.Unlock();
	};
	void AddFailed()//ʧ�ܼ���
	{
		m_cs.Lock();
		++m_nFailed;
		m_cs.Unlock();
	};
	void ReleaseFailed()//ʧ�ܼ���
	{
		m_cs.Lock();
		--m_nFailed;
		m_cs.Unlock();
	};
	//
	BOOL IsAllSuccessDone()
	{
		return (m_nAll == m_nSuccess) ? TRUE : FALSE;
	};
	//
	int GetSuccessCounter()
	{
		return m_nSuccess;
	};
	int GetFailedCounter()
	{
		return m_nFailed;
	};
private:
	volatile uint32_t m_nAll;
	volatile uint32_t m_nSuccess;
	volatile uint32_t m_nFailed;
	CrossCriticalSection m_cs;
};

