// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <queue>
#include <map>
#include <list>

#include "query_database.h"

using namespace std;

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


queue <PER_IO_OPERATION_DATA> queueOfIoData;
map <int, SOCKET> session;
map <int, int> userChatInGroup;

void handleMessHomeInterface();
void handleMessSignup();
void handleMessLogin();
void handleMessLogout();
void handleChatUser();
void handleChatGroup();
void addUserToSession(const char *, SOCKET);


int main(){
	QueryDatabase qd;
	cout << qd.checkGroupName("aba") <<endl;
	_getch();
	return 0;
}

//DWORD WINAPI theadHandleRecvFromServer(LPVOID completionPortID) {
//	/* Lắng nghe recve từ GetQueuedCompletionStatus() */
//	/* Nhận dữ liệu và đầy vào queue of struct chứa thông tin trong thông điệp của server * nếu là recv/
//
//	/* nếu là send thì lại lắng nghe */
//
//}


void threadHandleDataFromServer() {

	MESS mess;
	PER_HANDLE_DATA perHandeData;
	PER_IO_OPERATION_DATA perIoData;
	LPPER_IO_OPERATION_DATA pointIoData;
	while (true)
	{
		//wait();
		if (queueOfIoData.size() != 0) {
			pointIoData = &(queueOfIoData.front());
			perIoData.recvBytes = pointIoData->recvBytes;
			memcpy(perIoData.buffer, pointIoData->buffer, perIoData.recvBytes);
			perIoData.dataBuff.buf = perIoData.buffer;
			perIoData.dataBuff.len = perIoData.recvBytes;
			perIoData.sentBytes = 0;
			perIoData.operation = pointIoData->operation;
			perIoData.socket = pointIoData->socket;
			perHandeData.socket = perIoData.socket;
			queueOfIoData.pop();
			//signal();
			memcpy(&mess, perIoData.buffer, MESS_SIZE);
			// biến mess là struct chứa thông tin mà client gửi lên
			// nếu cần thông tin của socket hiện tại đang sử lí thì dùng perIoData.socket
			int typeMess = mess.typeMess, typeData = mess.typeData;
			char* buff = mess.payLoad;
			switch (typeMess) {
				case 0:
					handleMessHomeInterface();
					break;
				case 1:
					handleMessSignup();
					break;
				case 2:
					handleMessLogin();
					break;
				case 3:
					handleMessLogout();
				case 4:
					handleChatUser();
				case 5:
					handleChatGroup();
			}
		}
		//signal();

	}
}

void handleMessHomeInterface() {

}

void handleMessSignup() {

}

void handleMessLogin() {

}

void handleMessLogout() {

}

void handleChatUser() {

}

void handleChatGroup() {

}

void addUserToSession(const char * username, SOCKET socket){
	//int userId = getUserIdByUserName(username);
	//map[userId] = socket;
}