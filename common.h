#pragma once
#include<stdio.h>
#include<thread>
#include<iostream>
#include<string>
#include<functional>
#include<algorithm>
#include<mutex>
#include<stdio.h>

#define FLAG printf("[%s,%d] \n",__FUNCTION__,__LINE__)
#define DATE_LEN 1024


#pragma warning(disable:4996)










typedef char     s8;
typedef unsigned char   u8;
typedef short           s16;
typedef unsigned short  u16;
typedef int    s32;
typedef unsigned long DWORD;
#ifdef _MSC_VER
typedef __int64            s64;
typedef unsigned __int64 u64;
#else
typedef long long        s64;
typedef unsigned long long u64;
#endif
//兼容32流的 u32 定义
#if defined(_WIN64) || defined(WIN64) ||defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64)
typedef unsigned int    u32;
#define KD_PTR          void*
#else
typedef unsigned long    u32;
#define KD_PTR           u32
#endif