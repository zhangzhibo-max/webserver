
#include <stdio.h>
#include <thread>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <mutex>
#include <fstream>
#include <map>
#include <regex>
#include <stdlib.h>
#include <locale>
#include <codecvt>
#include <memory>


#pragma comment(lib,"wininet.lib")
#include "ws.h"
#include "common.h"
#include "libwebsockets.h"
#include "public.h"


void Send_addMsg()
{
	std::string str;
	std::string* send = new std::string;
	while (true)
	{
		printf(">");
 		std::cin >> str;
		printf("\n");
		//send = str;
		send->append(str);
		if(theCentWs->get_handle())
		{
			s32 n = lws_write((lws*)theCentWs->get_handle(), (unsigned char*)send->c_str(),send->length(), LWS_WRITE_TEXT);
			if (n == send->length())
			{
				printf("Send finish. \n");
			}
			else
			{
				printf("error. \n");
			}
			send->clear();
		}
		//lws_callback_on_writable((lws*)m_connectwsi.handle);	
	}
}


ws::ws()
{
	memset(&m_connectwsi, 0, sizeof(m_connectwsi));
	if (m_connectwsi.RecvBuf == NULL)
	{
		m_connectwsi.RecvBuf = new s8[DATE_LEN];
		memset(m_connectwsi.RecvBuf, 0, DATE_LEN);
	}
	if (m_connectwsi.SendBuf == NULL)
	{
		m_connectwsi.SendBuf = new s8[DATE_LEN];
		memset(m_connectwsi.SendBuf, 0, DATE_LEN);
	}
	Sleep(2000);
	std::thread send_thrad(Send_addMsg);
	send_thrad.detach();
}


ws::~ws()
{
	if (m_connectwsi.SendBuf)
		delete[] m_connectwsi.SendBuf;
	if (m_connectwsi.RecvBuf)
		delete[] m_connectwsi.RecvBuf;
}



void ws::Recv_addMsg(long wsi, s8* recvbuf, u32 dwlen)
{
	FLAG;
	if (wsi != 0 && recvbuf)
	{
		s8 szRecvBuff[1024] = "";
		std::string szTempBuff;
		szTempBuff.clear();
		szTempBuff.append(recvbuf);		
		//strncpy_gbk(szRecvBuff, szTempBuff,strlen(szTempBuff));
		 
		printf("%s \n", string_to_gbk(szTempBuff.c_str()));
		//memcpy(m_connectwsi.RecvBuf, recvbuf, dwlen);
	}
	//lws_write((lws*)wsi, (unsigned char*)m_connectwsi.RecvBuf, dwlen, LWS_WRITE_TEXT);
}




void ws::set_handle(long wsi)
{
	if(wsi!=0)
	m_connectwsi.handle = wsi;
}

void ws::clear_handle()
{
	m_connectwsi.handle = 0;
}

/*
std::string ws::getSendbuf()
{
	if(strlen(m_connectwsi.SendBuf)!=0)
	{
		std::string str(m_connectwsi.SendBuf);
			return str;
	}
}
*/