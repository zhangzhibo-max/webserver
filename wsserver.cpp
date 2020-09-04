#include "wsserver.h"


static int protocol_my_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
	THandle* ptData = (THandle*)user;
	switch (reason)
	{
	case LWS_CALLBACK_ESTABLISHED:       
	{
		theCentWs->set_handle((long)wsi);
		lwsl_notice("Client connect!\n");
	}
	break;
	case LWS_CALLBACK_CLOSED:
		lwsl_notice("Client disconnect!\n");
		theCentWs->clear_handle();
		break;
	case LWS_CALLBACK_RECEIVE:           
	{
		ptData->fin = lws_is_final_fragment(wsi);
		ptData->bin = lws_frame_is_binary(wsi);
		//printf("%s", (char*)in);
		theCentWs->Recv_addMsg((long)wsi, (s8*)in, len);


	}
	break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
	{
// 
// 		std::string strSend = theCentWs->getSendbuf();
// 		
// 
//  		int n = lws_write(wsi, (unsigned char*)strSend.c_str(), strSend.length(), LWS_WRITE_TEXT);
// 		if (n == strSend.length())
// 		{
// 			printf("Send finish. \n");
// 		}
		lws_rx_flow_control(wsi, 1);
	//	strSend.clear();
	}
	break;
	}

	return 0;
}

void startserver()
{
	while(1)
	{
		int  nRet = 0;
		if (theCentWsServer->GetListenSocket())
		{
			nRet = lws_service(theCentWsServer->GetListenSocket(), 1000);
			if (nRet > 0)
			{
				printf("GetMsg");
			}
		}
	}

}

wsserver::wsserver()
{
	FLAG;
	bIslisten = false;
	memset(&m_info, 0, sizeof(m_info));
	m_lws = NULL;

}
static const struct lws_http_mount mount = {
	/* .mount_next */		NULL,		/* linked-list "next" */
	/* .mountpoint */		"/",		/* mountpoint URL */
	/* .origin */			"./mount-origin",  /* serve from dir */
	/* .def */			"index.html",	/* default filename */
	/* .protocol */			NULL,
	/* .cgienv */			NULL,
	/* .extra_mimetypes */		NULL,
	/* .interpret */		NULL,
	/* .cgi_timeout */		0,
	/* .cache_max_age */		0,
	/* .auth_mask */		0,
	/* .cache_reusable */		0,
	/* .cache_revalidate */		0,
	/* .cache_intermediaries */	0,
	/* .origin_protocol */		LWSMPRO_FILE,	/* files in a dir */
	/* .mountpoint_len */		1,		/* char count */
	/* .basic_auth_login_file */	NULL,

};


THandle g_handle;
static struct lws_protocols protocols[] = {
	{
		"ws", protocol_my_callback, sizeof(THandle), 1024*1024, NULL, & g_handle, 0
	},
	{
		NULL, NULL, 0 
	}
};

bool wsserver::init()
{
	FLAG;
	lwsl_user("LWS minimal ws server | visit http://localhost:7681 (-s = use TLS / https)\n");
	m_info.port = 52136;
	m_info.mounts = &mount;
	m_info.protocols = protocols;
	m_info.vhost_name = "localhost";
	m_info.options = 0;
	startlisten();


	std::thread ws_thread(startserver);
	ws_thread.detach();
	return true;
}

bool wsserver::startlisten()
{
	m_lws = lws_create_context(&m_info);
	if (!m_lws) {
		lwsl_err("lws init failed\n");
		bIslisten = false;
		return false;
	}
	bIslisten = true;
	return true;
}


bool wsserver::destroy()
{
	lws_context_destroy(m_lws);
	return true;
}


