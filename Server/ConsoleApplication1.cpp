// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ctime>
#include <chrono>
using namespace std;

int main()
{
	cout << "\n=== step 1 - set the dll ===" << endl;
	SOCKET serverSocket, acceptSocket;
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
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // bind the winsock to a TCP protocol
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at socket" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "socket is ok" << endl;
	}

	cout << "\n=== step -3 bind the socket to the ip and port number ===" << endl;
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		cout << "Binding failed" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		cout << "bind is ok" << endl;
	};

	cout << "\n=== step -4 initiate listen ===" << endl;
	if (listen(serverSocket, 1) == SOCKET_ERROR)
		cout << "listen() error listening in socket" << endl;
	else
		cout << "listening is OK, waiting for connections...." << endl;

	cout << "\n=== step -5 Accept connection ===" << endl;

	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		cout << "Accept failed" << endl;
		WSACleanup();
		return -1;
	}
	else 
		cout << "Accept connection" << endl;

	cout << "\n=== step -6 chat with client ===" << endl;
	char buffer[200];
	int bytecount;
	while (true) {
		bytecount = recv(acceptSocket, buffer, 200, 0);
		if (strcmp(buffer, "leave") == 0) {

			break;
		}
		if (bytecount > 0) {
			std::time_t now = std::time(nullptr);

			// Use localtime_s to convert time_t to tm
			std::tm now_tm;
			localtime_s(&now_tm, &now); // Safer alternative to localtime

				cout << "client: " << buffer <<" " << now_tm.tm_hour << ":"
					<< now_tm.tm_min << ":"
					<< now_tm.tm_sec << endl;
			
		}
		else {
			WSACleanup();
		}
		
	
		
		cin.getline(buffer, 200);
		if (strcmp(buffer, "leave") == 0) {
			bytecount = send(acceptSocket, "leave", 200, 0);

			break;
		}
		bytecount = send(acceptSocket, buffer, 200, 0);
		if (bytecount > 0)
			cout << "------------------------------" << endl;
		else
			WSACleanup();
		
	}
	
	cout << "room closed" << endl;
	closesocket(serverSocket);
	closesocket(acceptSocket);
		WSACleanup();
		system("pause");

}

