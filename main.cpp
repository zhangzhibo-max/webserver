#include"center.h"



int main()
{
	SetConsoleOutputCP(65001);//set output encoding to UTF8
	center::InitInstance();
	center::AllocGlobal();
	center* pData = getglobal();
	if (pData)
	{
		pData->Init();
	}

	g_Exist = false;
	while ((!g_Exist))
	{
		Sleep(500);
	}
	getglobal()->Release();
	center::FreeGlobal();
	center::releaseInstance();
	return 0;
}



// using namespace std;
// int main()
// {
// 	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	//printf("客户端嵌套字已经打开!\n");
// 	int send_len = 0;
// 	int recv_len = 0;
// 	//定义服务端套接字，接受请求套接字
// 	SOCKET s_server;
// 	//服务端地址客户端地址
// 	SOCKADDR_IN server_addr;
// 	//初始化套接字库
// 	WSADATA wsadata;
// 	WSAStartup(0x22, &wsadata);
// 	//填充服务端地址信息
// 	//填充服务端信息
// 	server_addr.sin_family = AF_INET;
// 	server_addr.sin_addr.S_un.S_addr = inet_addr("10.67.76.23");
// 	server_addr.sin_port = htons(28703);
// 	string msgstr = "";
// 	//创建套接字
// 	s_server = socket(AF_INET, SOCK_STREAM, 0);
// 	int nTimeout = 120000;
// 	//设置接收超时为1000ms
// 	if (SOCKET_ERROR == setsockopt(s_server, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(int)))
// 	{
// 		fprintf(stderr, "Set SO_RCVTIMEO error !\n");
// 	}
// 	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
// 		cout << "服务器连接失败！" << endl;
// 		closesocket(s_server);
// 		//释放DLL资源
// 		WSACleanup();
// 		return 0;
// 	}
// 	else {
// 		cout << "服务器连接成功！" << endl;
// 	}
// 	//发送,接收数据
// 	cout << "开始传输桌面信息:" << endl;
// 	/*while (1) {*/
// 		char send_buf[4096] = "POST /api/v1/manage/system/login HTTP/1.1 \n\
// 			Content - Type: application/json \n\
// 			User - Agent : PostmanRuntime / 7.26.3\n  \
// 			Accept : */*\n \
// 			Cache-Control: no-cache\n \
// 			Postman-Token: 215ee3ab-6567-4ae8-a3da-ac2aa910edd0\n \
// 			Host: 10.67.76.23:28703\n \
// 			Accept-Encoding: gzip, deflate, br\n \
// 			Content-Length: 42 \r\n\r\n"; \
// 			std::string str = "{\"user_name\": \"admin\",\"password\": \"admin\"}";
// 		strcat(send_buf, str.c_str());
// 
// 		cout << "发送:"<<send_buf << endl;
// 		send_len = send(s_server, send_buf, strlen(send_buf), 0);
// 		if (send_len < 0) {
// 			cout << "发送失败！" << endl;
// 			closesocket(s_server);
// 			//释放DLL资源
// 			WSACleanup();
// 			return 0;
// 		}
// 		else {
// 			cout << "发送成功！" << endl;
// 		}
// 		char recv_buf[1025] = "";
// 		recv_len = recv(s_server, recv_buf, 1025, 0);
// 		if (recv_len < 0) {
// 			cout << "接收失败！" << endl;
// 			closesocket(s_server);
// 			//释放DLL资源
// 			WSACleanup();
// 			return 0;
// 		}
// 		else {
// 			cout << recv_buf << endl;
// 			cout << "接收成功！" << endl;
// 		}
// 	/*}*/
// 	//关闭套接字
// 	closesocket(s_server);
// 	//释放DLL资源
// 	WSACleanup();
// 	return 0;
// }
