#include "center.h"


center::center()
{
	m_pCwserver = NULL;
	m_pCws = NULL;
	//����
}

center::~center()
{
	//�ͷ���
}

bool center::InitInstance()
{
	FLAG;
	//��ʼ����־ģ��
	return true;
}

bool center::releaseInstance()
{
	FLAG;
	//�ͷ���־ģ��
	return true;
}

bool center::start()
{
	return true;
}

bool center::AllocGlobal()
{
	FLAG;
	if (g_center == NULL)
	{
		g_center = new center;
	}
	return true;
}

bool center::FreeGlobal()
{
	if (g_center)
	{
		g_center->Release();
		delete g_center;
	}
	return true;
}

bool center::Init()
{
	FLAG;
	if (m_pCwserver == NULL)
		m_pCwserver = new wsserver;
	Sleep(1000);
	m_pCwserver->init();


	Sleep(1000);
	if (m_pCws == NULL)
		m_pCws = new ws;

	
	return true;
}

bool center::Release()
{	
	if (m_pCws)
		delete m_pCws;
	if (m_pCwserver)
		delete m_pCwserver;	
	return true;
}

center* getglobal()
{
	return g_center;
}
