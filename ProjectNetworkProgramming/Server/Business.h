#pragma once
#include "DataIO.h"

map<SOCKET, int> mapSocktoId; // map socket->id
map<int, SOCKET> mapIdtoSock; // map id->socket
map<int, int> mapIdUsertoIdGroup; //map idUser->idGroup
QueryDatabase DB;

void handleMessHomeInterface(LPPER_IO_OPERATION_DATA &pointIoData, MESS mess) {
	LPMESS messSend;
	int typeMess = 0;
	int typeData;
	char payLoad[1000];
	switch (mess.typeData)
	{
	case 0:
	{
		/*return 2 list userOnlien and groupofUser*/
		typeData = 0;
		sendCLient(pointIoData, messSend, typeMess, typeData, DB.getListUserOnl(mapSocktoId[pointIoData->socket]));
		typeData = 1;
		sendCLient(pointIoData, messSend, typeMess, typeData, DB.getListGroupOfUser(mapSocktoId[pointIoData->socket]));
	}
	break;
	case 1:
		/*logout*/
		removeSocket(pointIoData->socket); // remove map
		break;
	case 2:
	{
		typeData = 2;
		if (!DB.checkGroupName(mess.payLoad)) {
			payLoad[0] = '1';
			payLoad[1] = '\0';
		}
		else {
			payLoad[0] = '0';
			payLoad[1] = '\0';
			DB.createGroup(mess.payLoad, DB.getUsernameById(mapSocktoId[pointIoData->socket]), 1);
		}
		sendCLient(pointIoData, messSend, typeMess, typeData, payLoad);
	}
	break;
	case 3:
	{
		/* chat 1-1*/
		typeMess = 3;
		typeData = 0;
		int idSend = mapSocktoId[pointIoData->socket];
		int idRecv = DB.getUserIdByUserName(mess.payLoad);
		char userSend[30];
		strcpy(userSend, DB.getUsernameById(idSend));
		if (!DB.checkChat11(idSend, idRecv)) {
			// Never ever message
			DB.createGroup(userSend, mess.payLoad, 0);
			//payLoad[strlen(payLoad)] = '\0'; can false
		}
		else {
			strcpy(payLoad, DB.getMessGroup(DB.getIdGroup11(userSend, mess.payLoad), 20));
			//payLoad[strlen(payLoad)] = '\0'; can false
		}
		/* map<idUser,idGroup>
		with idUser = idSend,
		idGroup = DB.getIdGroup11(userSend,mess.payLoad);
		*/
		sendCLient(pointIoData, messSend, typeMess, typeData, payLoad);
	}
	break;
	case 4:
	{
		typeMess = 4;
		typeData = 3;
		strcpy(payLoad, DB.getMessGroup(DB.getIdGroup1n(mess.payLoad), 20));
		//payLoad[strlen(payLoad)] = '\0'; can false
		sendCLient(pointIoData, messSend, typeMess, typeData, payLoad);
	}
	default:
		break;
	}

}

void handleMessSignup(LPPER_IO_OPERATION_DATA &pointIoData, MESS mess) {
	char username[30], password[30];
	// get username and password in payload
	stringstream ss(mess.payLoad);
	ss >> username >> password;

	LPMESS messSend;
	int typeMess, typeData;
	char buff[1000], result[1000];
	if (!DB.checkUserName(username)) {
		/*Username  ?� ???c s? d?ng */
		buff[0] = '\0'; // payload = NULL
		craffMessage(messSend, result, 1, 1, buff);
		pointIoData->dataBuff.len = MESS_SIZE;
		strcpy(pointIoData->dataBuff.buf, result);
		WSASend(pointIoData->socket, &(pointIoData->dataBuff), 1, NULL, 0, &(pointIoData->overlapped), NULL); // Send
	}
	else {
		buff[0] = '\0'; // payload = NULL
		DB.addUser(username, password); // th�m v�o CSDL
		craffMessage(messSend, result, 1, 0, buff);
		pointIoData->dataBuff.len = MESS_SIZE;
		strcpy(pointIoData->dataBuff.buf, result);
		WSASend(pointIoData->socket, &(pointIoData->dataBuff), 1, NULL, 0, &(pointIoData->overlapped), NULL); // Send
	}

}

void handleMessLogin(LPPER_IO_OPERATION_DATA &pointIoData, MESS mess, int &coutLock) {
	char username[30], password[30];
	// get username and password in payload
	stringstream ss(mess.payLoad);
	ss >> username >> password;

	LPMESS messSend;
	int typeMess, typeData;
	char buff[1000], result[1000];
	// Log in => typeMess = 2, payLoad = NULL
	typeMess = 2; buff[0] = '\0';

	if (!DB.checkUserName(username)) {
		/*typeData = 4; payLoad[] = NULL; (Login failed, this acount not exist)*/
		typeData = 4;
	}
	else {
		if (!DB.checkStatus(DB.getUserId(username))) {
			/*typeData = 2; payLoad[] = NULL; (Login failed, this acount is blocked)*/
			typeData = 2;
		}
		else {
			if (!DB.checkPassword(username, password)) {
				if (coutLock >= 3) {
					/*login failed three times => lock account*/
					DB.lockAccount(username);
				}
				else {
					/* typeData = 1; payLoad[] = NULL; (Login failed, enter wrong password)*/
					typeData = 1;
					coutLock++;
				}
			}
			else {
				if (checkUserOnline(username)) {
					/*typeData = 3; payLoad[] = NULL; (Login failed, the account is logged in elsewhere)*/
					typeData = 3;
				}
				else {
					/*  typeData = 0; payLoad[] = NULL; (log in success)*/
					typeData = 0;
					coutLock = 0;
					// map<id, socket>; 
				}
			}
		}
	}

	// return Mess to Client
	sendCLient(pointIoData, messSend, typeMess, typeData, buff);

}

void handleMessLogout(LPPER_IO_OPERATION_DATA &pointIoData, MESS mess) {
	LPMESS messSend;
	int typeMess = 3;
	int typeData;
	char buff[1000];
	buff[0] = '\0';
	if (removeSocket(pointIoData->socket)) {
	typeData = 0;
	}
	else {
	typeData = 1;
	}
	sendCLient(pointIoData, messSend, typeMess, typeData, buff);
}

void handleChatuser(LPPER_IO_OPERATION_DATA &pointIoData, MESS mess, int &coutMess11) {
	LPMESS messSend;
	int typeMess = 4;
	int idSend = mapSocktoId[pointIoData->socket];
	int idGroup = mapIdUsertoIdGroup[idSend];
	switch (mess.typeData)
	{
	case 0:
	{
		int typeData = 0;
		DB.pushMess(mess.payLoad, idGroup, idSend);
		vector <int> listId;
		DB.getListIdUser(listId, idGroup);
		vector <int>::iterator first_user;
		for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
			pointIoData->socket = mapIdtoSock[*first_user];
			sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad);
		}
	}
	break;
	case 1:
	{
		int typeData = 1;
		sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad);
	}
	break;
	case 2:
	{
		int typeData = 2;
		coutMess11++;
		char payLoad[1000];
		strcpy(payLoad, DB.getMessGroup(idGroup, 20 * coutMess11));
		sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad);
	}
	break;
	default:
		break;
	}
}

void handleChatGroup(LPPER_IO_OPERATION_DATA &pointIoData, MESS mess, int &coutMess1n) {
	LPMESS messSend;
	int typeMess = 4;
	int idSend = mapSocktoId[pointIoData->socket];
	int idGroup = mapIdUsertoIdGroup[idSend];
	switch (mess.typeData)
	{
	case 0:
	{
		int typeData = 0;
		DB.pushMess(mess.payLoad, idGroup, idSend);
		vector <int> listId;
		DB.getListIdUser(listId, idGroup);
		vector <int>::iterator first_user;
		for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
			pointIoData->socket = mapIdtoSock[*first_user];
			sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad);
		}
	}
	break;
	case 1:
	{
		int typeData = 1;
		sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad);
	}
	case 2:
	{
		int typeData = 0;
		coutMess1n++;
		char payLoad[1000];
		strcpy(payLoad, DB.getMessGroup(idGroup, 20 * coutMess1n));
		sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad);
	}
	case 3:
	{
		int typeData = 3;
		char userInvite[30], userBeInvited[30], nameGroup[30];
		char payLoad[100];
		stringstream ss(mess.payLoad);
		ss >> userInvite >> nameGroup >> userBeInvited;
		strcpy(payLoad, userInvite);
		strcat(payLoad, " ");
		strcat(payLoad, nameGroup);
		//payLoad[strlen(payLoad)] = '\0'; can false
		pointIoData->socket = mapIdtoSock[DB.getUserIdByUserName(userInvite)]; // change Socket to WSASend
		sendCLient(pointIoData, messSend, typeMess, typeData, mess.payLoad); // WSASend
	}
	break;
	case 4:
	{
		int typeData = 4;
		char status[2];
		char nameGroup[30];
		stringstream ss(mess.payLoad);
		ss >> status >> nameGroup;
		int idGroup = DB.getIdGroup1n(nameGroup);
		int agree = atoi(status);
		if (agree == 0) {
			DB.insertUserToGroup(idSend, idGroup);  // inser User in Group
			vector <int> listId;
			DB.getListIdUser(listId, idGroup);
			vector <int>::iterator first_user;
			for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
				pointIoData->socket = mapIdtoSock[*first_user];
				sendCLient(pointIoData, messSend, typeMess, typeData, DB.getUsernameById(idSend));
			}
		}
		else {
			//nothing follow struct message
		}
	}
	break;
	default:
		break;
	}
}
