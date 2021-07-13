#include"public.h"
#pragma warning(disable:4996)



char* string_to_gbk(const char* chUTF)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, chUTF, -1, NULL, 0);
	unsigned short* wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCCH)chUTF, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	delete[]wszGBK;
	return szGBK;
}


std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1]; 
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
















#include "windows.h"
#include <string.h>
#include <stdlib.h>
#include <locale.h>

char* GB18030ToUTF_8(const char* pText, int pLen);
char* UTF_8ToGB18030(const char* pText, int pLen);
void GB18030ToUnicode(wchar_t* pOut, const char* gbBuffer);
void UnicodeToGB18030(char* pOut, unsigned short* uData);
void UTF_8ToUnicode(wchar_t* pOut, const char* pText);
void UnicodeToUTF_8(char* pOut, wchar_t* pText);

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
