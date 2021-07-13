#pragma once
#include"common.h"
#include"center.h"

typedef struct tagConnectWsi
{
	long handle;
	s8* RecvBuf;
	s8* SendBuf;
}TConnectWsi;

class ws
{
public:
	ws();
	~ws();
	void Recv_addMsg(long wsi,s8* recvbuf,u32 dwlen);

	void set_handle(long wsi);
	long get_handle() { if (m_connectwsi.handle)return m_connectwsi.handle; else return 0; }
	void clear_handle();
private:
	TConnectWsi m_connectwsi;
	std::mutex m_lock;
};

