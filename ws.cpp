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

#include<xstring>

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


using std::wstring;
// 转码
// ASCII to Unicode
wstring AtoW(const std::string& str)
{
	//unicode的长度
	int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	//分配内存
	wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * nwLen);

	if (pUnicode)
	{
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, nwLen);
		wstring wstr = pUnicode;
		free(pUnicode);
		return wstr;
	}

	return nullptr;

}

// Unicode to ASCII
std::string WtoA(const wstring& wstr)
{
	int naLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	char* pAssii = (char*)malloc(sizeof(char) * naLen);

	if (pAssii)
	{
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, naLen, nullptr, nullptr);
		std::string str = pAssii;
		free(pAssii);
		return str;
	}

	return nullptr;
}

// utf8 to Unicode
wstring U2W(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * nwLen);

	if (pUnicode)
	{
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, nwLen);
		wstring wstr = pUnicode;
		free(pUnicode);
		return wstr;
	}

	return nullptr;
}

// Unicode to utf8
std::string W2U(const wstring& wstr)
{
	int naLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pAssii = (char*)malloc(sizeof(char) * naLen);

	if (pAssii)
	{
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, naLen, nullptr, nullptr);
		std::string str = pAssii;
		free(pAssii);
		return str;
	}

	return nullptr;
}


// ASCII to utf8
std::string A2U(const std::string& str)
{
	return W2U(AtoW(str));

}

// utf8 to ASCII

std::string U2A(const std::string& str)
{
	return WtoA(U2W(str));
}



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
		send->append(A2U(str));
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







char* GBKToUTF8(const char* chGBK)
{
	DWORD dWideBufSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)chGBK, -1, NULL, 0);
	wchar_t* pWideBuf = new wchar_t[dWideBufSize];
	wmemset(pWideBuf, 0, dWideBufSize);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)chGBK, -1, pWideBuf, dWideBufSize);

	DWORD dUTF8BufSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pWideBuf, -1, NULL, 0, NULL, NULL);

	char* pUTF8Buf = new char[dUTF8BufSize];
	memset(pUTF8Buf, 0, dUTF8BufSize);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pWideBuf, -1, pUTF8Buf, dUTF8BufSize, NULL, NULL);

	delete[]pWideBuf;
	return pUTF8Buf;
}




#include "windows.h"

#include <string.h>
#include <stdlib.h>
#include <locale.h>
char* GB18030ToUTF_8(const char* pText, int pLen);
char* UTF_8ToGB18030(const char* pText, int pLen);
void GB18030ToUnicode(wchar_t* pOut, const char* gbBuffer);
void UnicodeToGB18030(char* pOut, unsigned short* uData);
void UTF_8ToUnicode(wchar_t* pOut, const char* pText);


char* GB18030ToUTF_8(const char* pText, int pLen)
{
	char buf[1024];
	char* rst = new char[pLen + (pLen >> 2) + 2];

	memset(buf, 0, 1024);

	memset(rst, 0, pLen + (pLen >> 2) + 2);

	int i = 0;

	int j = 0;

	while (i < pLen)
	{
		if (*(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;
			GB18030ToUnicode(&pbuffer, pText + i);
			UnicodeToUTF_8(buf, &pbuffer);
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j + 1] = buf[1];
			tmp = rst[j + 2] = buf[2];
			j += 3;
			i += 2;
		}
	}

	strcpy(&rst[j], "/0");
	return rst;
}

void GB18030ToUnicode(wchar_t* pOut, const char* gbBuffer)
{
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
}

void UnicodeToUTF_8(char* pOut, wchar_t* pText)
{
	char* pchar = (char*)pText;
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));

	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);

	pOut[2] = (0x80 | (pchar[0] & 0x3F));
}


char* UTF_8ToGB18030(const char* pText, int pLen)

{
	char* newBuf = new char[pLen];

	char Ctemp[4];

	memset(Ctemp, 0, 4);

	int i = 0;

	int j = 0;

	while (i < pLen)
	{
		if (pText[i] > 0)
		{
			newBuf[j++] = pText[i++];
		}
		else
		{

			wchar_t Wtemp;

			UTF_8ToUnicode(&Wtemp, pText + i);

			UnicodeToGB18030(Ctemp, (unsigned short*)&Wtemp);

			newBuf[j] = Ctemp[0];

			newBuf[j + 1] = Ctemp[1];

			i += 3;

			j += 2;

		}

	}
	strcpy(&newBuf[j], "/0");

	return newBuf;

}

void UTF_8ToUnicode(wchar_t* pOut, const char* pText)

{
	char* uchar = (char*)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);

	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
}


void UnicodeToGB18030(char* pOut, unsigned short* uData)
{
	::WideCharToMultiByte(CP_ACP, NULL, LPCWSTR(uData), 1, pOut, sizeof(WCHAR), NULL, NULL);
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
		 ;
		printf("%s \n", UTF_8ToGB18030(GBKToUTF8(szTempBuff.c_str()),szTempBuff.length()) );

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
