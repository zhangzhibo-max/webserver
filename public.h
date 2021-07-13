#pragma once

#include <Windows.h>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include <xstring>
#include <windows.h>
#include <io.h>
#include <wininet.h>

using std::string;
using std::wstring;

char* string_to_gbk(const char* chUTF);
