#pragma once
#include"ws.h"
#include"wsserver.h"
#include"global.h"
#include"common.h"


class ws;
class wsserver;


class center
{
public:

	static bool InitInstance();
	static bool releaseInstance();
	static bool AllocGlobal();
	static bool FreeGlobal();


	bool Init();
	bool Release();

	bool start();

	wsserver* getwsserver() { return m_pCwserver; }
	ws* getws() { return m_pCws; };
private:

	center();
	~center();

	ws* m_pCws;
	wsserver* m_pCwserver;
};

center* getglobal();

#ifndef theCent
#define	theCent		getglobal()
#define theCentWs   getglobal()->getws()
#define theCentWsServer getglobal()->getwsserver()
#endif