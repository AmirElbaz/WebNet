
#include <iostream>
#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;

int main()
{
	cout << "\n=== step 1 - set the dll ===" << endl;
	SOCKET clientSocket;
	int port = 55555; //server default port number
	WSADATA wsaData; // windows socket api data object sent as referense to collect data about the connection
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2); // parsing the version 2.2 to 16 bits word and generate the versioning word expected by WSAStartup
	wsaerr = WSAStartup(wVersionRequested, &wsaData); // initialize the winsock
	if (wsaerr != 0) {
		cout << "The winsock dll was not found!" << endl;
		return 0;
	}
	else {
		cout << "the winsock dll was found" << endl;
		cout << "status: " << wsaData.szSystemStatus << endl;
	}

	cout << "\n=== step 2 - set up server socket ===" << endl;

	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // bind the winsock to a TCP protocol
	if (clientSocket == INVALID_SOCKET) {
		cout << "Error at socket" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "socket is ok" << endl;
	}

	cout << "\n=== step 3 - Connect with server ===" << endl;
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		cout << "\nclient connect() failed to connect" << endl;
		WSACleanup();
		return 0;

	}
	else {
		cout << "client connect() is OK" << endl;
		cout << "client can start sending and recieving data..." << endl;

	}
	system("pause");
	WSACleanup();
	return 0;
}
