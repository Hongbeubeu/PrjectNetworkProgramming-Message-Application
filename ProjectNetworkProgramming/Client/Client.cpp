// client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector> 
#include <process.h>
#include "windows.h"

using namespace std;

typedef struct {
	int typeMess;
	int typeData;
	char payLoad[1000];
} MESS, *LPMESS;

vector<char *> notifications;
char nameOfChatting[40];
bool chatting = false;
bool wait = false;

#define MESS_SIZE sizeof(MESS)
#define BUFF_SIZE 2048

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#pragma warning(disable : 4018)

unsigned __stdcall selecetThreadListenMessForChatUser(void *param);
unsigned __stdcall selecetThreadListenMessForChatGroup(void *param);


void craftMessage(LPMESS mess, char* buffMess, int typeMess, int typeData, char* buff) {
	mess->typeMess = typeMess;
	mess->typeData = typeData;
	if (buff == NULL)
		mess->payLoad[0] = 0;
	else {
		memcpy(mess->payLoad, buff, strlen(buff));
		mess->payLoad[strlen(buff)] = 0;
	}
	memcpy(buffMess, mess, MESS_SIZE);
	return;
}

// fuction wrap of send() 
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

// Function get infor for menu()
// pramater:
//      userName: pointer to array contain username enter from user
//      passWord: pointer to array contain password enter from user
void getAndCheckInfoForMenu(char* userName, char* passWord) {

	while (1) {
		cout << "\n\tEnter your username:";
		cin >> userName;
		cout << "\n\tEnter your password:";
		cin >> passWord;
		bool check = true;
		for (int i = 0; i < strlen(userName); ++i) {
			if ((userName[i] >= 48 && userName[i] <= 57) || (userName[i] >= 65 && userName[i] <= 90) || (userName[i]) >= 97 && userName[i] <= 122 || userName[i] == ' ');
			else if (strlen(userName) <= 30);
			else {
				cout << "Your username wrong, enter again!" << endl;
				check = false;
				break;
			}
		} // end of for 1
		if (check == true) {
			for (int i = 0; i < strlen(passWord); ++i) {
				if ((passWord[i] >= 48 && passWord[i] <= 57) || (passWord[i] >= 65 && passWord[i] <= 90) || (passWord[i]) >= 97 && passWord[i] <= 122 || passWord[i] == ' ');
				else if (strlen(passWord) <= 30);
				else {
					cout << "Your password wrong, enter again!" << endl;
					check = false;
					break;
				}
			} // end of for 2
		}
		if (check == true) break;

	} // end of while
	return;
}


// 	Funtion print result for result from server
//	Pramater:
//		result:	contain value of code of result
void printResultForMenu(int result) {
	if (result == 0) {
		cout << "Logged in successfully" << endl;
	}
	if (result == 1) {
		cout << "Login failed, enter wrong password" << endl;
	}
	if (result == 2) {
		cout << "Login failed, this acount  is blocked" << endl;
	}
	if (result == 3) {
		cout << "Login failed, the account is logged in elsewhere" << endl;
	}
	if (result == 4) {
		cout << "Login failed, this acount not exist" << endl;
	}
	if (result == 5) {
		cout << "Sign up success" << endl;
	}
	if (result == 6) {
		cout << "Sign up fail, this acount existed" << endl;
	}
	return;
}

// Function handle choice Log in
// pramater:
//      client: value socket connet with server
//      buffContainUserAndPass: pointer to array contain user and password entered from user
bool login(SOCKET client, char* buffContainUserAndPass) {
	char buffMess[MESS_SIZE];
	MESS mess;

	// Sent mess to notify server start login
	craftMessage(&mess, buffMess, 1, 0, NULL);
	sendData(client, buffMess, MESS_SIZE);

	// Sent mess cotain username and password
	craftMessage(&mess, buffMess, 1, 1, buffContainUserAndPass);
	sendData(client, buffMess, MESS_SIZE);

	//recv result from server
	recvData(client, buffMess, MESS_SIZE);

	memcpy(&mess, buffMess, MESS_SIZE);
	printResultForMenu(mess.typeData);

	if (mess.typeData == 0)
		return true;
	else
		return false;

}

// Function handle choice sing up
// pramater:
//      client: contain value socket connet with server
//      buffContainUserAndPass: pointer to array contain user and password entered from user
void singup(SOCKET client, char* buffContainUserAndPass) {
	char buffMess[MESS_SIZE];
	MESS mess;

	// Sent mess to notify server start signup
	craftMessage(&mess, buffMess, 2, 0, NULL);
	sendData(client, buffMess, MESS_SIZE);

	// Sent mess cotain username and password
	craftMessage(&mess, buffMess, 2, 1, buffContainUserAndPass);
	sendData(client, buffMess, MESS_SIZE);

	//recv result from server
	recvData(client, buffMess, MESS_SIZE);

	memcpy(&mess, buffMess, MESS_SIZE);

	printResultForMenu(mess.typeData);

	return;

}

// fuction display menu and handle choice of user
//	Pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
void menu(SOCKET client, int* status) {
	char buffContainUserAndPass[100], userName[40], passWord[40];
	int choice; //(1 is sigup, 2 is login);

	system("cls");

	//  Display fuctions for user
	cout << "Chioce fuction you want!" << endl;
	cout << "\t1. Log In\n\t2.Sign Up\n\t3.Exit" << endl;
	while (true) {
		char input[100];
		cout << "\n\tEnter your choice: ";
		cin >> input;
		if (strlen(input) == 1 && input[0] < '4' && input[0] > '0') {
			choice = input[0] - '0';
			break;
		}
		else {
			cout << "Your choice is wrong, please re-enter your choice" << endl;
		}
	}
	if (choice == 3) {// shut down program
		char buffMess[MESS_SIZE];
		MESS mess;
		// Sent mess to notify server start signup
		craftMessage(&mess, buffMess, 5, 0, NULL);
		sendData(client, buffMess, MESS_SIZE);
		exit(0);
	}
	getAndCheckInfoForMenu(userName, passWord);
	memcpy(buffContainUserAndPass, userName, strlen(userName));
	buffContainUserAndPass[strlen(userName)] = ' ';
	char* point = buffContainUserAndPass;
	memcpy(point + strlen(userName) + 1, passWord, strlen(passWord));
	buffContainUserAndPass[strlen(userName) + 1 + strlen(passWord)] = 0;
	switch (choice) {
	case 1:
		if (login(client, buffContainUserAndPass) == true)
			*status = 2; // Go to Home interface
		else
			*status = 1; // Back to menu interface
		break;
	case 2:
		singup(client, buffContainUserAndPass);
		*status = 1;    // Back to menu
		break;
	}
}


//	Function handle choice of user in home
//	Pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
// 		choice: contain value choice of user: = 0 in_homepage, = 1 logout, = 2 creata group, = 3 choice user or group to chat
// 		choiceToChat: contain index is displayed of user or group
//		typeMess: contain value of typeMess in MESS
//		typeData: contain value of typeData in MESS
//		buff: pointer to array contain value get from user
//		listOfUserOnl: pointer to array contain list of user online
//		listOfGroup: pointer to arrar contain list of group of user
int handleHomeInterface(SOCKET client, int* status, int choice, int choiceToChat, char* buff, vector<char*> listOfUserOnl, vector<char*> listOfGroup) {
	int typeMess = 0, typeData;
	MESS mess;
	char buffMess[MESS_SIZE];
	switch (choice) // 	choice: contain value choice of user: = 0 in_homepage, = 1 logout, = 2 creata group, = 3 choice user to chat
					// = 4 choice group to chat, = 5 answer invite

	{
	case 0:
		typeData = 0;
		buff[0] = 0;
		break;
	case 1:
		typeData = 1;
		buff[0] = 0;
		*status = 1;
		break;
	case 2:
		typeData = 2;
		*status = 2;
		break;
	case 3:
		memcpy(buff, nameOfChatting, strlen(nameOfChatting));
		buff[strlen(nameOfChatting)] = 0;
		chatting = true;
		typeData = 3;
		*status = 3;
		break;
	case 4:
		memcpy(buff, nameOfChatting, strlen(nameOfChatting));
		buff[strlen(nameOfChatting)] = 0;
		chatting = true;
		typeData = 4;
		*status = 4;
		break;
	case 5:
		typeData = 5;
		*status = 2;
		break;
	}

	craftMessage(&mess, buffMess, typeMess, typeData, buff);
	sendData(client, buffMess, MESS_SIZE);

	if (choice == 0 || choice == 2)

		switch (choice)
		{
		case 0:
			char* taken;
			do {
				recvData(client, buffMess, MESS_SIZE);
				memcpy(&mess, buffMess, MESS_SIZE);
				if (mess.typeData == 0) {
					taken = strtok(mess.payLoad, " ");
					while (taken != NULL)
					{
						char* userName = new char[strlen(taken) + 1];
						memcpy(userName, taken, strlen(taken));
						userName[strlen(taken)] = 0;
						listOfUserOnl.push_back(userName);
						taken = strtok(NULL, " ");
					}

				}
				else if (mess.typeData == 1) {
					taken = strtok(mess.payLoad, " ");
					while (taken != NULL)
					{
						char* group = new char[strlen(taken) + 1];
						memcpy(group, taken, strlen(taken));
						group[strlen(taken)] = 0;
						listOfGroup.push_back(group);
						taken = strtok(NULL, " ");
					}
				}
				else if (mess.typeMess == 0 && mess.typeData == 4) {
					char* notifi = new char[strlen(mess.payLoad) + 1];
					memcpy(notifi, mess.payLoad, strlen(mess.payLoad) + 1);
					notifications.push_back(notifi);
				}

			} while (!(mess.typeMess == 0 && mess.typeData == 2));
			break;
		case 2:
			if (typeMess == 0 && typeData == 3) {
				if (mess.payLoad[0] == '0')
					return true;
				else if (mess.payLoad[0] == '1')
					return false;
				else
					return false;
			}

			break;
		}
	return true;
}

// Function print result for mess initialize home and create new group from user
// Pramater:
//		choiceOfUser: contain value choice of user
//		sign: value indicate the result from server
//		listOfUserOnl: pointer to array contain list of user online
//		listOfGroup: pointer to arrar contain list of group of user
void printResultForHome(int choiceOfUser, bool sign, vector<char*> listOfUserOnl, vector<char*> listOfGroup) {
	switch (choiceOfUser)
	{
		//	Print result for initialize home
	case 0:
		cout << "\nNumber of user online: " << listOfUserOnl.size();
		cout << "\nNumber of your group: " << listOfGroup.size();
		cout << "\nYou have " << notifications.size() << " new notitfications!";
		if (notifications.size() > 0) {
			int index = 1;
			cout << "\n\nList of your notifications:";
			char* taken;
			for (char* notifi : notifications) {
				taken = strtok(notifi, " ");
				cout << "\n\tIndex: " << index << " You was invited to group " << taken << " by user ";
				taken = strtok(NULL, " ");
				cout << taken;
				++index;
			}
		}
		if (listOfUserOnl.size() > 0) {
			int index = 1;
			cout << "\n\nList of user onl:";
			for (char* userName : listOfUserOnl) {
				cout << "\n\tIndex: " << index << " ,name of user: " << userName;
				++index;
			}
		}
		if (listOfGroup.size() > 0) {
			int index = 1;
			cout << "\n\nList of your group:";
			for (char* userName : listOfUserOnl) {
				cout << "\n\tIndex: " << index << " ,name of group: " << userName;
				++index;
			}
		}
		break;

		//	Print result for create new group
	case 2:
		if (sign == true)
			cout << "\n\t Created Group success!" << endl;
		else
		{
			cout << "\n\t created group fail!" << endl;
		}
		Sleep(5000);
	}
	return;
}

//  Function catch envent in home
// 	choiceOfUser: contain value choice of user: = 0 in_homepage, = 1 logout, = 2 creata group, = 3 choice user to chat
// = 4 choice group to chat, = 5 answer invite
void catchEventsForHome(int* choiceOfUser, char* buff, vector<char*> listOfUserOnl, vector<char*> listOfGroup) {
	char input[100];
	bool check = false;
	do {
		cout << "\nEnter your function you want: ";
		cin >> input;
		//User enter the characters from the keyboard to decive function to implement";
		//Form of input from user";
		//  Logout: '0'";
		//  Create Group: '1'"
		//  Choice notification to answer: "2 indexOfNotification" (indexOfNotifiCation is index was displayed with notification) ";
		//  Choice user to chat: "3 indexOfUser" (indexOfUser is index was displayed with user)";
		//  Choice group to chat: "4 indexOfGroup" indexOfGroup is index was displayed with group)";
		if (strlen(input) == 1) {     // event logut or create group
			if (input[0] == '0') {    // Event logut
				*choiceOfUser = 1;
				break;
			}
			else if (input[0] == '1') // Event create group
			{
				//Get name of group from user and check
				while (true) {
					cout << "\nEnter name of new group: ";
					bool checkInput = true;
					cin >> input;
					for (int i = 0; i < strlen(input); i++) {
						if (((input[i] >= 48 && input[i] <= 57) || (input[i] >= 65 && input[i] <= 90) || (input[i]) >= 97 && input[i] <= 122) && input[i] != ' ');
						else {
							checkInput = false;
							break;
						}
					}
					if (strlen(input) > 0 && strlen(input) <= 30);
					else {
						checkInput = false;
						break;
					}
					if (checkInput == true)
						break;
					else
						cout << "\nYour enter wrong, re-enter name of new group";
				} //End of while
				*choiceOfUser = 2;
				memcpy(buff, input, strlen(input));
				buff[strlen(input)] = 0;
				break;
			}
		}
		else // Event choice notification to answarn or choice user to chat or choice group to chat
		{
			if (input[0] >= '2' && input[0] <= '4' && input[1] == ' ') {
				bool checkDigital = true;
				for (int i = 2; i < strlen(input); ++i) {
					if (input[i] >= '9' || input[i] <= '0') {
						check = false;
						break;
					}
				}
				if (checkDigital == true) {
					int index = atoi(&input[2]);

					if ((input[0] - '0') == 2) {  //Event Choice notification to answer
						if (index <= notifications.size() && index >= 1) {
							char* taken;
							taken = strtok(notifications[index - 1], " ");
							cout << "\n\tUser " << taken << " invited to group ";
							taken = strtok(NULL, " ");
							cout << taken;
							while (true) {
								cout << "\n\tEnter your answer (0 mean Yes, 1 mean No):";
								cin >> input;
								if (strlen(input) == 1 && input[0] >= '0' && input[0] <= '1') {
									buff[0] = input[0];
									buff[1] = ' ';
									taken = strtok(notifications[index - 1], " ");
									taken = strtok(NULL, " ");
									memcpy(&buff[2], taken, strlen(taken));
									buff[strlen(taken) + 2] = 0;
									*choiceOfUser = 5;
									notifications.erase(notifications.begin()+(index - 1));
									break;
								}
								cout << "\nYour enter wrong, re-enter you answer";
							}
						}
					}
					else if (input[0] - '0' == 3) { //choice user to chat
						if (index <= listOfUserOnl.size() && index >= 0) {
							*choiceOfUser = 3;
							memcpy(buff, listOfUserOnl[index - 1], strlen(listOfUserOnl[index - 1]) + 1);
							buff[strlen(listOfUserOnl[index - 1])] = 0;
							break;
						}
					}
					else   //Event choice group to chat
						if (index <= listOfGroup.size() && index >= 0) {
							*choiceOfUser = 4;
							memcpy(buff, listOfGroup[index - 1], strlen(listOfGroup[index - 1]) + 1);
							buff[strlen(listOfGroup[index - 1])] = 0;
							break;
						}
				}
			}
		}

		cout << "\nYour input is wrong, re-enter input";

	} while (true);
}

// Functin handle diplay homepage for user and catch function user want
// pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
void homeInterface(SOCKET client, int* status) {

	// 	choiceOfUser: contain value choice of user: = 0 in_homepage, = 1 logout, = 2 creata group, = 3 choice user to chat
	// = 4 choice group to chat, = 5 answer invite
	int choiceOfUser = 0;
	vector <char*> listOfUserOnl, listOfGroup;
	bool sign;
	char buff[1000];

	system("cls");

	cout << "\nUser enter the characters from the keyboard to decive function to implement";
	cout << "\nForm of input from user";
	cout << "\nLogout: '0'";
	cout << "\nCreate Group: '1'";
	cout << "\nChoice notification to answer: \"2 indexOfNotification\" (indexOfNotifiCation is index was displayed with notification) ";
	cout << "\nChoice user to chat: \"3 indexOfUser\" (indexOfUser is index was displayed with user)";
	cout << "\nChoice group to chat: \"4 indexOfGroup\" indexOfGroup is index was displayed with group)";

	//  Initialize home and get information about user online and group of user from server
	sign = handleHomeInterface(client, status, choiceOfUser, 0, NULL, listOfUserOnl, listOfGroup);
	printResultForHome(choiceOfUser, sign, listOfUserOnl, listOfGroup);

	//  catch event from user and handle this
	catchEventsForHome(&choiceOfUser, buff, listOfUserOnl, listOfGroup);
	sign = handleHomeInterface(client, status, choiceOfUser, 0, buff, listOfUserOnl, listOfGroup);
	printResultForHome(choiceOfUser, sign, listOfUserOnl, listOfGroup);

	return;
}

//  Function catch envents in chat group
//  Pramater:
//	    choice: = 0 sent mess, = 1 back home, = 2 show more mess in history, 3 invite user
//      buff: contain infor enter from user
void catchEvent(int* choice, char buff[]) {
	cout << "\nPress ESC to back home";
	cout << "\nPress F1 to add friend in group";
	cout << "\nPress F2 to see more mess";
	bool kt = true;
	int i = 0;
	while (kt == true) {
		if (_kbhit()) {
			char kitu = _getch();
			buff[i] = kitu;
			printf("%c", kitu);
			i++;
			switch (kitu)
			{
			case 27: //catch ESC
			{
				*choice = 1;
				buff[i] = '\0';
				kt = false;

			}
			break;
			case 59: //catch F1
			{
				printf("User was invited:");
				*choice = 3;
				int j = 0;
				buff[0] = '\0';
				bool check = true;
				while (check == true) {
					if (_kbhit()) {
						char kitu = _getch();
						buff[j] = kitu;
						printf("%c", kitu);
						j++;
						switch (kitu)
						{
						case 13: //catch ENTER
						{
							buff[j] = '\0';
							check = false;
						}
						break;
						case 8: //catch BackSpace
						{
							buff[i - 1] = '\0';
							i--;
						}
						break;
						default:
							break;
						}
					}
				}
				kt = false;
			}
			break;
			case 60: //catch F2
			{
				*choice = 2;
				buff[i] = '\0';
				kt = false;
			}
			break;
			case 13: //catch ENTER
			{
				choice = 0;
				buff[i] = '\0';
				kt = false;
			}
			break;
			case 8: //catch BackSpace
			{
				buff[i - 1] = '\0';
				i--;
			}
			break;
			default:
				break;
			}


		}
	}
}

//  Fuction handle chat user 1 - 1 
//	Pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
//	    choiceOfUser: = 0 sent mess, = 1 back home, = 2 show more mess in history
//		typeMess: contain value of typeMess in MESS
//		typeData: contain value of typeData in MESS
//		buff: pointer to array contain value get from user
void handleChatUser(SOCKET client, int* status, int choice, char* buff) {
	int typeMess = 3, typeData;
	MESS mess;
	char buffMess[MESS_SIZE];
	switch (choice) {
	case 0:
		typeData = 0;
		break;
	case 1:
		typeData = 1;
		buff[0] = 0;
		chatting = false;
		*status = 1;
		break;
	case 2:
		typeData = 2;
		break;
	}
	craftMessage(&mess, buffMess, typeMess, typeData, buff);
	sendData(client, buffMess, MESS_SIZE);
}

//  Function display chat 1 - 1
//	Pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
void chatUserInterface(SOCKET client, int* status) {
	char buff[1000];
	int choiceOfUser = 0;

	system("cls");

	cout << "\nUser enter the characters from the keyboard to decive function to implement";
	cout << "\nPress ESC to back home";
	cout << "\nPress F2 to see more mess";
	cout << "\nPress charaters except ESC and F2 to chat";

	//  Start thread listen message from server and handle it
	if (_beginthreadex(0, 0, selecetThreadListenMessForChatUser, (void*)client, 0, 0) == 0) {
		printf("Create thread failed with error %d\n", GetLastError());
		return;
	}

	do {
		catchEvent(&choiceOfUser, buff);
		handleChatUser(client, status, choiceOfUser, buff);
		if (choiceOfUser == 2)
			system("cls");
	} while (choiceOfUser != 0);

	return;
}

// Function handle chat group
//	Pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
//	    choice: = 0 sent mess, = 1 back home, = 2 show more mess in history, 3 invite user
//		typeMess: contain value of typeMess in MESS
//		typeData: contain value of typeData in MESS
//		buff: pointer to array contain value get from user
void handleChatGroup(SOCKET client, int* status, int choice, char* buff) {
	int typeMess = 4, typeData;
	MESS mess;
	char buffMess[MESS_SIZE];
	switch (choice) {
	case 0:
		typeData = 0;
		break;
	case 1:
		chatting = false;
		typeData = 1;
		buff[0] = 0;
		*status = 1;
		break;
	case 2:
		typeData = 2;
		buff[0] = 0;
		break;
	case 3:
		typeData = 3;
		int n = strlen(buff);
		buff[n] = ' ';
		memcpy(&buff[n + 1], nameOfChatting, strlen(nameOfChatting));
		buff[strlen(nameOfChatting) + n + 1] = 0;
		break;
	}
	craftMessage(&mess, buffMess, typeMess, typeData, buff);
	sendData(client, buffMess, MESS_SIZE);
}

//  Function handle display of chat group
//	Pramater:
//      client: contain value socket connet with server
//		status: pointer to variable contain value to control program
void chatGroupInterface(SOCKET client, int* status) {
	char buff[1000];
	int choiceOfUser = 0; //  choiceOfUser: = 0 sent mess, = 1 back home, = 2 show more mess in history, 3 invite user
	system("cls");

	cout << "\nUser enter the characters from the keyboard to decive function to implement";
	cout << "\nPress ESC to back home";
	cout << "\nPress F2 to see more mess";
	cout << "\nPress F1 to add friend in group";
	cout << "\nPress charaters except ESC, F1 and F2 to chat";

	// Start threadex to recv and handle mess from server
	_beginthreadex(0, 0, selecetThreadListenMessForChatGroup, (void *)client, 0, 0);

	do {
		catchEvent(&choiceOfUser, buff);
		handleChatGroup(client, status, choiceOfUser, buff);
		if (choiceOfUser == 2)
			system("cls");
	} while (choiceOfUser != 0);

}

int main(int argc, char* argv[])
{
	//Step 1: Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
		cout << "Version is not supported\n" << endl;

	//Step 2: Construct socket	
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Step 3: Specify server address
	// Giá trị port và addr server lấy từ tham số dòng lệnh thông qua mảng con chỏ argv[]
	u_short serverPort = (u_short)strtoul(argv[2], NULL, 0);

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	//Step 4: Request to connect server
	if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		cout << "\nCannot connect server! Error: " << WSAGetLastError() << endl;
		_getch();
		return 0;
	}
	printf("\nConnected server!");

	//Step 5: Communicate with server

	//buff is array contain content of request sent to server
	//sign is array contain status of acount recv from server

	char buff[BUFF_SIZE];
	int status, signResult, numOfUserOnl, numOfGroup;
	status = 1;
	while (true) {
		switch (status) {
		case 1:
			menu(client, &status);
			break;
		case 2:
			homeInterface(client, &status);
			break;
		case 3:
			chatUserInterface(client, &status);
			break;
		case 4:
			chatGroupInterface(client, &status);
			break;
		}
	}

	//Step 6: Close socket

	shutdown(client, SD_BOTH);
	closesocket(client);

	//Step 7: Terminate Winsock
	WSACleanup();
	_getch();


	return 0;
}

// The thread handle connect from client
unsigned __stdcall selecetThreadListenMessForChatUser(void *param) {
	SOCKET clientSocket = (SOCKET)param;
	SOCKET client[1];
	fd_set readfds; //use initfds to initiate readfds at the begining of every loop step
	char buffMess[MESS_SIZE];
	MESS mess;
	int nEvents;
	bool ret;

	client[0] = clientSocket;
	FD_ZERO(&readfds);
	while (1) {

		FD_SET(client[0], &readfds);
		nEvents = select(0, &readfds, 0, 0, 0);
		if (nEvents < 0) {
			printf("\nError! Cannot poll sockets: %d", WSAGetLastError());
			return 0;
		}
		//receive data from clients and convert to MESS
		recvData(clientSocket, buffMess, MESS_SIZE);
		memcpy(&mess, buffMess, MESS_SIZE);

		if (mess.typeData = 0)  // mess contain message from other user
			cout << "\n\tFrom " << nameOfChatting << " : " << mess.payLoad;
		else if (mess.typeData == 1) // mess notification other user out of chat
		{
			cout << "\n\t(Your friend out of chat !)" << endl;
		}
		else if (mess.typeMess == 0 && mess.typeData == 4) {    // if mess contain notification invite
			char* notifi = new char[strlen(mess.payLoad) + 1];
			memcpy(notifi, mess.payLoad, strlen(mess.payLoad) + 1);
			notifications.push_back(notifi);
		}
		else if (mess.typeData == 2)
			break;  // break if user back to home 

	}
	return 0;
}

// The thread handle connect from client
unsigned __stdcall selecetThreadListenMessForChatGroup(void *param) {

	SOCKET clientSocket = (SOCKET)param;
	SOCKET client[1];
	fd_set readfds; //use initfds to initiate readfds at the begining of every loop step
	MESS mess;
	char buffMess[MESS_SIZE];
	int nEvents;
	bool ret;

	client[0] = clientSocket;
	FD_ZERO(&readfds);
	//Step 5: Communicate with clients
	while (1) {

		FD_SET(client[0], &readfds);

		nEvents = select(0, &readfds, 0, 0, 0);
		if (nEvents < 0) {
			printf("\nError! Cannot poll sockets: %d", WSAGetLastError());
			return 0;
		}
		//receive data from clients and convert to MESS
		recvData(clientSocket, buffMess, MESS_SIZE);
		memcpy(&mess, buffMess, MESS_SIZE);

		if (mess.typeMess == 0 && mess.typeData == 4) { // handle mess contain notification invite from server
			char* notifi = new char[strlen(mess.payLoad) + 1];
			memcpy(notifi, mess.payLoad, strlen(mess.payLoad) + 1);
			notifications.push_back(notifi);
		}
		else
		{
			switch (mess.typeData)
			{
			case 0: // mess contain message from other people in group 
				char userName[40], content[1000], *taken;
				taken = strtok(mess.payLoad, " ");
				memcpy(userName, taken, strlen(taken));
				userName[strlen(taken)] = 0;
				taken = strtok(NULL, " ");
				memcpy(content, taken, strlen(taken));
				content[strlen(taken)] = 0;
				cout << "\n\tFrom " << userName << " : " << content;
				break;
			case 1: // mess from server notification other user out of chat
				cout << "\n\t(User " << mess.payLoad << " out of chat)";
				break;
			case 2: // mess from server contain notification user out of chat
				return 0;
				break;
			case 3: // Mess from server contain name of people agree group
				cout << "\n\t User " << mess.payLoad << " join this group";
				break;
			}
		}

	}
	return 0;
}