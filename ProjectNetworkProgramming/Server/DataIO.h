#pragma once
#include "stdafx.h"
#include <stdio.h>    
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>
#include <process.h>  
#include <map> 
#include <queue> 
#include "QueryDatabase.h"

map<SOCKET, int> mapSocktoId; // map socket->id
map<int, SOCKET> mapIdtoSock; // map id
map<int, int> mapIdUsertoIdGroup; //map idUser->idGroup

typedef struct {

	int typeMess;
	int typeData;
	char payLoad[1000];

} MESS, *LPMESS;

#define MESS_SIZE sizeof(MESS)

// Structure definition
typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF dataBuff;
	char buffer[MESS_SIZE];
	int bufLen;
	int recvBytes;
	int sentBytes;
	int operation;
	SOCKET socket;

} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct {
	SOCKET socket;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

// funciton no SQL


void craffMessage(LPMESS mess, char* buffMess, int typeMess, int typeData, char* buff) {
	mess->typeMess = typeMess;
	mess->typeData = typeData;
	memcpy(mess->payLoad, buff, strlen(buff));
	memcpy(buffMess, mess, MESS_SIZE);
	return;
}
//fuction wrap of send()
void sendData(SOCKET client, char* buff, int dataLength) {

	int index = 0, nLeft = dataLength, ret;

	while (nLeft > 0) {
		ret = send(client, &(buff[index]), nLeft, 0);
		if (ret == SOCKET_ERROR) {
			cout << "\n\tError: " << WSAGetLastError() << "when sen data to server" << endl;
			break;
		}

		index += ret;
		nLeft -= ret;
	}
	return;
}
void sendCLient(SOCKET socket, LPMESS mess, int typeMess, int typeData, char* buff) {
	char buffMess[1000];
	craffMessage(mess, buffMess, 2, 4, buff);
	sendData(socket, buffMess, MESS_SIZE); //send
}

