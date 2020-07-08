#pragma once
#include "DataIO.h"
#include "QueryDatabase.h"

QueryDatabase DB;

void handleMessHomeInterface(SOCKET socket, LPMESS mess) {
	int typeData, typeMess;
	char payLoad[1000];
	switch (mess->typeData)
	{
		case 0:
		{
			/*return 2 list userOnlien and groupofUser*/
			typeData = 0;
			for (map<int, SOCKET>::iterator it = mapIdtoSock.begin(); it != mapIdtoSock.end(); it++) {
				strcat(payLoad, DB.getUsernameById(it->first));
				strcat(payLoad, " ");
			}
			sendCLient(socket, mess, typeMess, typeData, payLoad);
			typeData = 1;
			sendCLient(socket, mess, typeMess, typeData, DB.getListGroupOfUser(mapSocktoId[socket]));
		}
		break;
		case 1:
		{
			/*logout*/
			map<int, SOCKET>::iterator it1;
			int id = mapSocktoId[socket];
			it1 = mapIdtoSock.find(id);
			mapIdtoSock.erase(it1);
			map<SOCKET, int>::iterator it2;
			it2 = mapSocktoId.find(socket);
			mapSocktoId.erase(it2);
		}
			break;
		case 2:
		{
			typeData = 2;
			if (!DB.checkGroupName(mess->payLoad)) {
				payLoad[0] = '1';
				payLoad[1] = '\0';
			}
			else {
				payLoad[0] = '0';
				payLoad[1] = '\0';
				DB.createGroup(mess->payLoad, DB.getUsernameById(mapSocktoId[socket]), 1);
			}
			sendCLient(socket, mess, typeMess, typeData, payLoad);
		}
		break;
		case 3:
		{
			/* chat 1-1*/
			typeMess = 3;
			typeData = 0;
			int idSend = mapSocktoId[socket];
			int idRecv = DB.getUserIdByUserName(mess->payLoad);
			char userSend[30];
			strcpy(userSend, DB.getUsernameById(idSend));
			if (!DB.checkChat11(idSend, idRecv)) {
				// Never ever message
				DB.createGroup(userSend, mess->payLoad, 0);
				//payLoad[strlen(payLoad)] = '\0'; can false
			}
			else {
				vector <char*> messGroup;
				DB.getMessGroup(messGroup, DB.getIdGroup11(idSend, idRecv), 20);
				vector <char*>::iterator first_user;
				for (first_user = messGroup.begin(); first_user != messGroup.end(); first_user++) {
					strcpy(payLoad, *first_user);
					sendCLient(socket, mess, typeMess, typeData, payLoad);
				}
			}
			/* map<idUser,idGroup>
			with idUser = idSend,
			idGroup = DB.getIdGroup11(userSend,mess->payLoad);
			*/
			mapIdUsertoIdGroup[idSend] = DB.getIdGroup11(idSend, idRecv);
		}
		break;
		case 4:
		{
			typeMess = 4;
			typeData = 3;
			vector <char*> messGroup;
			DB.getMessGroup(messGroup, DB.getIdGroup1n(mess->payLoad), 20);
			vector <char*>::iterator first_user;
			for (first_user = messGroup.begin(); first_user != messGroup.end(); first_user++) {
				strcpy(payLoad, *first_user);
				sendCLient(socket, mess, typeMess, typeData, payLoad);
			}
			//payLoad[strlen(payLoad)] = '\0'; can false
			/* map<idUser,idGroup>
			idGroup = DB.getIdGroup1n(const char* nameGroup);
			*/
			mapIdUsertoIdGroup[mapSocktoId[socket]] = DB.getIdGroup1n(mess->payLoad);
		}
		break;
		default:
			break;
	}

}

void handleMessSignup(SOCKET socket, LPMESS mess) {
	char username[30], password[30];
	// get username and password in payload
	stringstream ss(mess->payLoad);
	ss >> username >> password;

	LPMESS mess;
	int typeMess, typeData;
	char payLoad[1000], result[1000];
	if (!DB.checkUserName(username)) {
		/*Username  exited */
		typeMess = 2;
		typeData = 1;
		payLoad[0] = '\0';
		sendCLient(socket, mess, typeMess, typeData, payLoad);
	}
	else {
		typeMess = 2;
		typeData = 0;
		payLoad[0] = '\0';
		sendCLient(socket, mess, typeMess, typeData, payLoad);
	}
}

void handleMessLogin(SOCKET socket, LPMESS mess, int &coutLock) {
	char username[30], password[30];
	// get username and password in payload
	stringstream ss(mess->payLoad);
	ss >> username >> password;

	LPMESS mess;
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
				if (mapIdtoSock.count(DB.getUserId(username)) > 0) {
					/*typeData = 3; payLoad[] = NULL; (Login failed, the account is logged in elsewhere)*/
					typeData = 3;
				}
				else {
					/*  typeData = 0; payLoad[] = NULL; (log in success)*/
					typeData = 0;
					coutLock = 0;
					mapIdtoSock[DB.getUserIdByUserName(username)] = socket; //mapid->sock
					mapSocktoId[socket] = DB.getUserIdByUserName(username); //mapsock-id
				}
			}
		}
	}

	// return Mess to socket
	sendCLient(socket, mess, typeMess, typeData, buff);

}

/*void handleMessLogout(LPPER_IO_OPERATION_DATA &socket, MESS mess) {
LPMESS mess;
int typeMess = 3;
int typeData;
char buff[1000];
buff[0] = '\0';
if (removeSocket(socket)) {
typeData = 0;
}
else {
typeData = 1;
}
sendCLient(socket, mess, typeMess, typeData, buff);
}*/

void handleChatuser(SOCKET socket, LPMESS mess, int &coutMess11) {
	int typeMess = 4;
	int idSend = mapSocktoId[socket];
	int idGroup = mapIdUsertoIdGroup[idSend];
	switch (mess->typeData)
	{
	case 0:  // send Mess,
	{
		int typeData = 0;
		DB.pushMess(mess->payLoad, idGroup, idSend);
		vector <int> listId;
		DB.getListIdUser(listId, idGroup);
		vector <int>::iterator first_user;
		for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
			//check UserOnline to Send
			if (mapIdtoSock.count(*first_user) > 0){
					SOCKET socketTMP = mapIdtoSock[*first_user];
					sendCLient(socketTMP, mess, typeMess, typeData, DB.getUsernameById(idSend));
				}
			}
	}
	break;
	case 1: // out of chat
	{
		//send yourselft out of chat
		int typeData = 2;
		sendCLient(socket, mess, typeMess, typeData, mess->payLoad);
		//send to other user
		int typeData = 1;
		vector <int> listId;
		DB.getListIdUser(listId, idGroup);
		vector <int>::iterator first_user;
		//check Online
		for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
			if (mapIdtoSock.count(*first_user) > 0) {
					SOCKET socketTMP = mapIdtoSock[*first_user];
					sendCLient(socket, mess, typeMess, typeData, mess->payLoad);
				}
			}
	}
	break;
	case 2: //see more
	{
		int typeData = 0;
		coutMess11++;
		char payLoad[1000];
		vector <char*> messGroup;
		DB.getMessGroup(messGroup, idGroup, 20 * coutMess11);
		vector <char*>::iterator first_user;
		for (first_user = messGroup.begin(); first_user != messGroup.end(); first_user++) {
			strcpy(payLoad, *first_user);
			sendCLient(socket, mess, typeMess, typeData, payLoad);
		}
	}
	break;
	default:
		break;
	}
}

void handleChatGroup(SOCKET socket, LPMESS mess, int &coutMess1n) {
	int typeMess = 4;
	int idSend = mapSocktoId[socket];
	int idGroup = mapIdUsertoIdGroup[idSend];
	switch (mess->typeData)
	{
	case 0: // send Mess
	{
		int typeData = 0;
		DB.pushMess(mess->payLoad, idGroup, idSend);
		vector <int> listId;
		DB.getListIdUser(listId, idGroup);
		vector <int>::iterator first_user;
		for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
			if (mapIdtoSock.count(*first_user) > 0) {
					SOCKET socketTMP = mapIdtoSock[*first_user];
					sendCLient(socketTMP, mess, typeMess, typeData, DB.getUsernameById(idSend));
				}
			}
	}
	break;
	case 1: // user out of chat
	{
		int typeData = 1;
		vector <int> listId;
		DB.getListIdUser(listId, idGroup);
		vector <int>::iterator first_user;
		for (first_user = listId.begin(); first_user != listId.end(); first_user++) {
			if (mapIdtoSock.count(*first_user) > 0) {
					SOCKET socketTMP = mapIdtoSock[*first_user];
					sendCLient(socket, mess, typeMess, typeData, mess->payLoad);
				}
		}
		//send all people online group

	}
	case 2: // see more mess
	{
		int typeData = 0;
		coutMess1n++;
		char payLoad[1000];
		vector <char*> messGroup;
		DB.getMessGroup(messGroup, idGroup,  20 * coutMess1n);
		vector <char*>::iterator first_user;
		for (first_user = messGroup.begin(); first_user != messGroup.end(); first_user++) {
			strcpy(payLoad, *first_user);
			sendCLient(socket, mess, typeMess, typeData, payLoad);
		}

	}
	case 3: // typeMess = 0; typeData = 4 ; payLoad[] = 'nameOfUserInvite nameOfGroupToJoin' 
	{
		int typeMess = 0;
		int typeData = 4;
		char userIntive[30], userBeInvited[30], nameGroup[30];
		char payLoad[100];
		stringstream ss(mess->payLoad);
		ss >> nameGroup >> userBeInvited;
		int idRecv;
		strcpy(userIntive, DB.getUsernameById(idSend));
		strcpy(payLoad, userIntive);
		strcat(payLoad, " ");
		strcat(payLoad, nameGroup);
		//payLoad[strlen(userIntive)] = '\0'; can false
		// check Online to Send
		if (mapIdtoSock.count(DB.getUserId(userBeInvited)) > 0){
			// Send
			SOCKET socketTMP = mapIdtoSock[DB.getUserId(userBeInvited)];
			sendCLient(socketTMP, mess, typeMess, typeData, DB.getUsernameById(idSend));
		}
	}
	break;
	default:
		break;
	}
}
