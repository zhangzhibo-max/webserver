#pragma once

#include"libwebsockets.h"
#include"common.h"
#include"center.h"

typedef struct THandletag {
	lws_fop_fd_t fop_fd;
	int			   msg_count;
	unsigned char  buf[LWS_PRE + 1024*1024];
	int			   len;
	bool		   bin;
	bool		   fin;
}THandle;


class wsserver
{
public:
	wsserver();
	~wsserver() { destroy(); }
	bool init();
	bool startlisten ();

	lws_context* GetListenSocket() {return m_lws;}

	void Run();

	bool destroy();
private:
	bool bIslisten;
	lws_context* m_lws;
	lws_context_creation_info m_info;
};

