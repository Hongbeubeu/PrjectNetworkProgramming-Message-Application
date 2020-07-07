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

/*Khai bao Map,...*/



void wait() {
	while (semaphore == true) {}
	semaphore = true;
}

void signal() {
	semaphore = false;
}

bool checkUserOnline(const char *) {
	// check xem trong map session 
}

void craffMessage(LPMESS mess, char* buffMess, int typeMess, int typeData, char* buff) {
	mess->typeMess = typeMess;
	mess->typeData = typeData;
	memcpy(mess->payLoad, buff, strlen(buff));
	memcpy(buffMess, mess, MESS_SIZE);
	return;
}
void sendCLient(LPPER_IO_OPERATION_DATA &pointIoData, LPMESS mess, int typeMess, int typeData, char* buff) {
	char buffMess[1000];
	craffMessage(mess, buffMess, 2, 4, buff);
	pointIoData->dataBuff.len = MESS_SIZE;
	strcpy(pointIoData->dataBuff.buf, buffMess);
	WSASend(pointIoData->socket, &(pointIoData->dataBuff), 1, NULL, 0, &(pointIoData->overlapped), NULL); // Send
}

