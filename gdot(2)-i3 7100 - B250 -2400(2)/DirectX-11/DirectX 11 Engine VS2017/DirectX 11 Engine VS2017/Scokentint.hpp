#ifndef _SOCKET_INIT_H_
#define _SOCKET_INTI_H_
//#include <WinSock2.h>
#include <winsock.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

class ScoketInit
{
public:
	ScoketInit() {
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wasdata;
		if (WSAStartup(sockVersion, &wasdata) != 0) {
			printf("scpp\n");
		}
	}
	~ScoketInit() {
		WSACleanup();
	}
};
#endif
