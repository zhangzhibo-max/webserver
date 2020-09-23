#include "wsserver.h"

struct msg {
	void* payload; /* is malloc'd */
	size_t len;
};


struct per_session_data__minimal {
	struct per_session_data__minimal* pss_list;
	struct lws* wsi;
	int last; /* the last message number we sent */
};


struct per_vhost_data__minimal {
	struct lws_context* context;
	struct lws_vhost* vhost;
	const struct lws_protocols* protocol;

	struct per_session_data__minimal* pss_list; /* linked-list of live pss*/

	struct msg amsg; /* the one pending message... */
	int current; /* the current message number we are caching */
};
static void
__minimal_destroy_message(void* _msg)
{
	struct msg* msg =(struct msg*)_msg;

	free(msg->payload);
	msg->payload = NULL;
	msg->len = 0;
}

static int protocol_my_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
	THandle* ptData = (THandle*)user;
// 	struct per_session_data__minimal* pss =
// 		(struct per_session_data__minimal*)user;
// 	struct per_vhost_data__minimal* vhd =
// 		(struct per_vhost_data__minimal*)
// 		lws_protocol_vh_priv_get(lws_get_vhost(wsi),
// 			lws_get_protocol(wsi));
// 	int m;

	switch (reason)
	{
//	case LWS_CALLBACK_PROTOCOL_INIT:  //协议初始化
// 		vhd = (struct per_vhost_data__minimal*)lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),lws_get_protocol(wsi),sizeof(struct per_vhost_data__minimal));
// 		vhd->context = lws_get_context(wsi);
// 		vhd->protocol = lws_get_protocol(wsi);
// 		vhd->vhost = lws_get_vhost(wsi);
// 		break;
// 	case LWS_CALLBACK_ESTABLISHED:    //建立连接
		/* add ourselves to the list of live pss held in the vhd */
// 		lws_ll_fwd_insert(pss, pss_list, vhd->pss_list);
// 		pss->wsi = wsi;
// 		pss->last = vhd->current;
// 		break;

// 	case LWS_CALLBACK_CLOSED:		  //关闭连接
		/* remove our closing pss from the list of live pss */
// 		lws_ll_fwd_remove(struct per_session_data__minimal, pss_list,
// 			pss, vhd->pss_list);

// 		break;
	case LWS_CALLBACK_ESTABLISHED:       
	{
		theCentWs->set_handle((long)wsi);
		//lwsl_notice("Client connect!\n");
	}
	break;
	case LWS_CALLBACK_CLOSED:
	{
		lwsl_notice("Client disconnect!\n");
		theCentWs->clear_handle();
	}
	break;

	case LWS_CALLBACK_RECEIVE:           //收消息
	{
		ptData->fin = lws_is_final_fragment(wsi);
		ptData->bin = lws_frame_is_binary(wsi);
		//printf("%s", (char*)in);
		theCentWs->Recv_addMsg((long)wsi, (s8*)in, len);


// 		if (vhd->amsg.payload)
// 			__minimal_destroy_message(&vhd->amsg);
// 
// 		vhd->amsg.len = len;
		/* notice we over-allocate by LWS_PRE */
// 		vhd->amsg.payload = malloc(LWS_PRE + len);
// 		if (!vhd->amsg.payload) {
// 			lwsl_user("OOM: dropping\n");
// 			break;
// 		}
// 
// 		memcpy((char*)vhd->amsg.payload + LWS_PRE, in, len);
// 		vhd->current++;

		/*
		 * let everybody know we want to write something on them
		 * as soon as they are ready
		 */
// 		lws_start_foreach_llp(struct per_session_data__minimal**,
// 			ppss, vhd->pss_list) {
// 			lws_callback_on_writable((*ppss)->wsi);
// 		} lws_end_foreach_llp(ppss, pss_list);
		break;

	}
	break;

	case LWS_CALLBACK_SERVER_WRITEABLE:   //回消息
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
//	strSend.clear();

		lws_rx_flow_control(wsi, 1);
// 		if (!vhd->amsg.payload)
// 			break;
// 
// 		if (pss->last == vhd->current)
// 			break;
// 
// 		/* notice we allowed for LWS_PRE in the payload already */
// 		m = lws_write(wsi, ((unsigned char*)vhd->amsg.payload) +
// 			LWS_PRE, vhd->amsg.len, LWS_WRITE_TEXT);
// 		if (m < (int)vhd->amsg.len) {
// 			lwsl_err("ERROR %d writing to ws\n", m);
// 			return -1;
// 		}
// 
// 		pss->last = vhd->current;
		break;

	}
	default:
		break;
	}

	return 0;
}

void startserver()
{
	while(1)
	{
		theCentWsServer->Run();
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
	if (m_lws != NULL)
	{
		lws_context_destroy(m_lws);
		m_lws = NULL;
	}
	lwsl_user("LWS minimal ws server | visit http://localhost:7681 (-s = use TLS / https)\n");
	
	m_info.port = 52136;
	m_info.mounts = &mount;
	m_info.protocols = protocols;
	m_info.vhost_name = "localhost";
	m_info.options = LWS_SERVER_OPTION_VALIDATE_UTF8;
	m_info.gid = -1;
	m_info.uid = -1;
	m_info.iface = NULL;

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

std::mutex g_lock;
void wsserver::Run()
{
	g_lock.lock();
	if (m_lws != NULL)
	{
		lws_service(m_lws, 0);
	}
	g_lock.unlock();	
}

bool wsserver::destroy()
{
	lws_context_destroy(m_lws);
	return true;
}


