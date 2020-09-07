#include "ws.h"
#include "common.h"
#include"libwebsockets.h"
#include<stdio.h>
#include<thread>
#include<iostream>
#include<string>
#include<functional>
#include<algorithm>
#include<mutex>
#include<stdio.h>

#include <stdio.h>

#include <fstream>
#include <map>
#include <regex>
#include  <stdlib.h>

#include <locale>
#include <codecvt>
#include <memory>

#include <iostream>



#include <windows.h>
#include  <io.h>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")


std::mutex lock;
std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}




void Send_addMsg()
{
	std::string str;
	str.clear();
	std::string send;
	while (true)
	{
		printf(">");
 		std::cin >> str;
		printf("\n");
		//send = str;
		send = string_To_UTF8(str);
		if(theCentWs->get_handle())
		{
			s32 n = lws_write((lws*)theCentWs->get_handle(), (unsigned char*)send.c_str(),send.length(), LWS_WRITE_TEXT);
			if (n == send.length())
			{
				printf("Send finish. \n");
			}
		}
		str.clear();
		send.clear();
		//lws_callback_on_writable((lws*)m_connectwsi.handle);	
		std::cin.clear();
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
	if (wsi != 0)
	{
		printf("%s \n", recvbuf);
		//memcpy(m_connectwsi.RecvBuf, recvbuf, dwlen);
	}
	//lws_write((lws*)wsi, (unsigned char*)m_connectwsi.RecvBuf, dwlen, LWS_WRITE_TEXT);
}

#include <locale>
#include <codecvt>
#include <fstream>






//#include<cstdio>



void ws::set_handle(long wsi)
{
	if(wsi!=0)
	m_connectwsi.handle = wsi;
}

void ws::clear_handle()
{
	m_connectwsi.handle = 0;
}

// std::string ws::getSendbuf()
// {
// 	if(strlen(m_connectwsi.SendBuf)!=0)
// 	{
// 		std::string str(m_connectwsi.SendBuf);
// 			return str;
// 	}
// }
