// server.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Business.h"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <cstring>
#include <process.h>

#define SERVER_ADDR "10.0.0.1"
#define BUFF_SIZE 2048


#pragma warning(disable : 4018)
#pragma warning(disable : 4996)

#pragma comment(lib, "ws2_32.lib")

using namespace std;

typedef struct {
	int typeMess;
	int typeData;
	char payLoad[1000];
} MESS, *LPMESS;

// The thread handle connect from client
#define MESS_SIZE sizeof(MESS)
void recvData(SOCKET client, char* buff, int dataLength);

bool bussiness(SOCKET client) {
	MESS mess;
	char buffMess[MESS_SIZE];
	recvData(client, buffMess, MESS_SIZE);
	memcpy(&mess, buffMess, MESS_SIZE);
	if (mess.typeMess == 5 && mess.typeData == 0)\
		return false;
	switch (mess.typeMess) {
		case 0: {
			handleMessHomeInterface(, mess);
			break;
		}
		case 1: {
			handleMessSignup();
			break;
		}
		case 2: {
			handleMessLogin();
			break;
		}
		case 3: {
			handleMessLogout();
			break;
		}
		case 4: {
			handleChatuser();
			break;
		}

		case 5: {
			handleChatGroup();
			break;
		}
	}
	return true
}


unsigned __stdcall selecetThread(void *param) {

	++numberOfThread;
	SOCKET listenSock = (SOCKET)param;
	DWORD		nEvents = 0;
	DWORD		index;
	SOCKET		socks[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT	events[WSA_MAXIMUM_WAIT_EVENTS];
	WSANETWORKEVENTS sockEvent;

	socks[0] = listenSock;
	events[0] = WSACreateEvent(); //create new events
	nEvents++;

	// Associate event types FD_ACCEPT and FD_CLOSE
	// with the listening socket and newEvent   
	WSAEventSelect(socks[0], events[0], FD_ACCEPT | FD_CLOSE);

	SOCKET connSock;
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	int ret, i;

	for (i = 1; i < WSA_MAXIMUM_WAIT_EVENTS; i++) {
		socks[i] = 0;
	}
	while (1) {

		if (listenSock != 0) {
			int n = 0;
			for (int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; ++i) {
				if (socks[i] != 0)
					++n;
			}
			// if in this thread have maximum of client connect to server
			// listenSock is sent to new thread and the listenSock in this thread set = 0
			if (n == WSA_MAXIMUM_WAIT_EVENTS) {
				_beginthreadex(0, 0, selecetThread, (void *)listenSock, 0, 0); // start new thread when maximum of client in one thread
				listenSock = 0;
			}
		}
		//wait for network events on all socket
		index = WSAWaitForMultipleEvents(nEvents, events, FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED) {
			printf("WSAWaitForMultipleEvents() failed with error %d\n", WSAGetLastError());
			break;
		}

		index = index - WSA_WAIT_EVENT_0;
		WSAEnumNetworkEvents(socks[index], events[index], &sockEvent);

		if (sockEvent.lNetworkEvents & FD_ACCEPT) {
			if (sockEvent.iErrorCode[FD_ACCEPT_BIT] != 0) {
				printf("FD_ACCEPT failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
				break;
			}

			if ((connSock = accept(socks[index], (sockaddr *)&clientAddr, &clientAddrLen)) == SOCKET_ERROR) {
				printf("accept() failed with error %d\n", WSAGetLastError());
				break;
			}

			printf("You got a connection from %s\n", inet_ntoa(clientAddr.sin_addr)); /* prints client's IP */
			cout << "\tconnSocket: " << (int)connSock << endl;

			//Add new socket into socks array
			int i;
			if (nEvents == WSA_MAXIMUM_WAIT_EVENTS) {
				printf("\nToo many clients.");
				closesocket(connSock);
			}
			else
				for (i = 1; i < WSA_MAXIMUM_WAIT_EVENTS; i++)
					if (socks[i] == 0) {
						socks[i] = connSock;
						events[i] = WSACreateEvent();
						WSAEventSelect(socks[i], events[i], FD_READ | FD_CLOSE);
						nEvents++;
						break;
					}

			//reset event
			WSAResetEvent(events[index]);
		}

		if (sockEvent.lNetworkEvents & FD_READ) {
			//Receive message from client
			if (sockEvent.iErrorCode[FD_READ_BIT] != 0) {
				printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
				break;
			}

			ret = bussiness(socks[index]);
			if (ret == false) {
				closesocket(socks[index]);
				socks[index] = 0;
				socks[index] = 0;
				WSACloseEvent(events[index]);
				nEvents--;
			}
			else {
				WSAResetEvent(events[index]);
			}
		}
		if (sockEvent.lNetworkEvents & FD_CLOSE) {
			if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0) {
				printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_CLOSE_BIT]);
				break;
			}
			closesocket(socks[index]);
			socks[index] = 0;
			WSACloseEvent(events[index]);
			nEvents--;
		}
	}
	return 0;
}



int main(int argc, char* argv[])
{

	//Step 1: Initiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
		printf("Version is not supported\n");

	//Step 2: Construct socket	
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Step 3: Bind address to socket
	// set value of port from argv[1]
	u_short serverPort = (u_short)strtoul(argv[1], NULL, 0);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error! Cannot bind this address.");
		_getch();
		return 0;
	}

	//Step 4: Listen request from client
	if (listen(listenSock, 10)) {
		printf("Error! Cannot listen.");
		_getch();
		return 0;
	}

	printf("Server started!");

	//Step 5: Communicate with client

	cout << "\nListening new connect..." << endl;

	// start select thread
	_beginthreadex(0, 0, selecetThread, (void *)listenSock, 0, 0);
	Sleep(10000);
	while (numberOfThread != 0) {}

	//Step 5: Close socket
	closesocket(listenSock);

	//Step 6: Terminate Winsock
	WSACleanup();

	return 0;
}




// void craftMessage(LPMESS mess, char* buffMess, int typeMess, int typeData, char* buff){
//     mess->typeMess = typeMess;
//     mess->typeData = typeData;
//     if(buff == NULL)
//         mess->payLoad[0] = 0;
//     else{
//         memcpy(mess->payLoad, buff, strlen(buff));
//         mess->payLoad[strlen(buff)] = 0;
//     }
//     memcpy(buffMess, mess, MESS_SIZE);
//     return;
// }

// // fuction wrap of send() 
// void sendData(SOCKET client, char* buff, int dataLength) {

// 	int index = 0, nLeft = dataLength, ret;

// 	while (nLeft > 0) {
// 		ret = send(client, &(buff[index]), nLeft, 0);
// 		if (ret == SOCKET_ERROR) {
// 			cout << "\n\tError: " << WSAGetLastError() << "when sen data to server" << endl;
// 			break;
// 		}

// 		index += ret;
// 		nLeft -= ret;
// 	}
// 	return;
// }

 // fuction wrap of recv() 
 void recvData(SOCKET client, char* buff, int dataLength) {

 	int index = 0, nLeft = dataLength, ret;

 	while (nLeft > 0) {
 		ret = recv(client, &(buff[index]), nLeft, 0);
 		if (ret == SOCKET_ERROR) {
 			cout << "\n\tError: " << WSAGetLastError() << "when sen data to server" << endl;
 			break;
 		}

 		index += ret;
 		nLeft -= ret;

 		buff[dataLength] = 0;

 	}
 	return;
 }



