#include "stdafx.h"
#include "QueryDatabase.h"

#include <iostream>
using namespace std;
QueryDatabase::QueryDatabase() {

};

QueryDatabase::~QueryDatabase() {

};

/**
 * AddUser is used to add user into database
 * 
 * @param userName contain user name string of account
 * @param password contain password string of account 
 * @param status contain status of account
 * @return no return value
 */
void QueryDatabase::addUser(const char * userName, const char * password) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		
		SQLString userName(userName, strlen(userName));
		SQLString password(password, strlen(password));
		prep_stmt = con->prepareStatement("INSERT INTO user (userName, password, status) VALUES (?, ?, ?)");

		prep_stmt->setString(1, userName);
		prep_stmt->setString(2, password);
		prep_stmt->setInt(3, 0);
		prep_stmt->executeQuery();
		
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
 * getUserIdByUserName is used to get id of account has username
 * @param userName is userName of account is being get id
 * @return id of userName
 */
int QueryDatabase::getUserIdByUserName(const char *userName){
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		
		SQLString userName(userName, strlen(userName));
		prep_stmt = con->prepareStatement("SELECT id FROM user WHERE username = ?");
		prep_stmt->setString(1, userName);
		res = prep_stmt->executeQuery();
		if (res->next())
			return res->getInt("id");
		else 
			return -1;
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

/**
 * LockAccount is used to lock account
 * @param userName is userName of account is being locked
 * @return no return value
 */
void QueryDatabase::lockAccount(const char * userName) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		/* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString userName(userName, strlen(userName));
		prep_stmt = con->prepareStatement("UPDATE user SET status = 1 WHERE userName = ?");
		prep_stmt->setString(1, userName);
		prep_stmt->execute();
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* CreateGroup is used to createGroup
* @param userId is id of user insert to group
* @param groupId is id of group which user is invited to join
* @return no return value
*/
void QueryDatabase::createGroup(const char * groupName, const char * userName, const int type){
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;
		int groupId;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		/* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		if (type == 1) {
			int userId;
			SQLString groupName(groupName, strlen(groupName));
			SQLString userName(userName, strlen(userName));
			prep_stmt = con->prepareStatement("INSERT INTO group_chat (group_name, type) VALUES (?, ?)");
			prep_stmt->setString(1, groupName);
			prep_stmt->setInt(2, type);
			prep_stmt->executeQuery();

			/* Get id of the group that has been created*/
			prep_stmt = con->prepareStatement("SELECT id FROM group_chat WHERE group_name = ?");
			prep_stmt->setString(1, groupName);
			res = prep_stmt->executeQuery();
			if (res->next())
				groupId = res->getInt("id");

			prep_stmt = con->prepareStatement("SELECT id FROM user WHERE username = ?");
			prep_stmt->setString(1, userName);

			res = prep_stmt->executeQuery();
			if (res->next())
				userId = res->getInt("id");
			insertUserToGroup(userId, groupId);
		}
		else {
			int userId1;
			int userId2;
			char name[50];
			strcpy(name, groupName);
			strcat(name, userName);
			name[strlen(name)] = 0;
			SQLString groupName11(name, strlen(name));
			SQLString userName1(groupName, strlen(groupName));
			SQLString userName2(userName, strlen(userName));
			prep_stmt = con->prepareStatement("INSERT INTO group_chat (group_name, type) VALUES (?, ?)");
			prep_stmt->setString(1, groupName11);
			prep_stmt->setInt(2, type);
			prep_stmt->executeQuery();

			prep_stmt = con->prepareStatement("SELECT id FROM group_chat WHERE group_name = ?");
			prep_stmt->setString(1, groupName11);
			res = prep_stmt->executeQuery();
			if (res->next())
				groupId = res->getInt("id");

			prep_stmt = con->prepareStatement("SELECT id FROM user WHERE username = ?");
			prep_stmt->setString(1, userName1);
			res = prep_stmt->executeQuery();
			if (res->next())
				userId1 = res->getInt("id");

			prep_stmt = con->prepareStatement("SELECT id FROM user WHERE username = ?");
			prep_stmt->setString(1, userName2);
			res = prep_stmt->executeQuery();
			if (res->next())
				userId2 = res->getInt("id");

			insertUserToGroup(userId1, groupId);
			insertUserToGroup(userId2, groupId);
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* InsertUserToGroup is used to add user to groupChat
* @param userId is id of user insert to group
* @param groupId is id of group which user is invited to join
* @return no return value
*/
void QueryDatabase::insertUserToGroup(const int userId,const int groupId){
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");

		prep_stmt = con->prepareStatement("INSERT INTO group_user (user_id, group_id) VALUES (?, ?)");

		prep_stmt->setInt(1, userId);
		prep_stmt->setInt(2, groupId);
		prep_stmt->executeQuery();

		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* checkUserName is used to check username is used or not
* @param userName is userName of account is being to check
* @return true when userName is not used
* @return false when userName is used
*/
bool QueryDatabase::checkUserName(const char * userName){
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString userName(userName, strlen(userName));
		prep_stmt = con->prepareStatement("SELECT count(*) as counter FROM user WHERE username = ?");
		prep_stmt->setString(1, userName);
		res = prep_stmt->executeQuery();
		if (res->next()){
			if (res->getInt("counter") == 1)
				return false;
			else 
				return true;
		} else {
			return true;
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* checkPassword is used to check password is correct or not
* @param userName is userName of account is being to check
* @param password is password of account is being to check
* @return true when userName is not used
* @return false when userName is used
*/
bool QueryDatabase::checkPassword(const char * userName, const char * password) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString userName(userName, strlen(userName));
		prep_stmt = con->prepareStatement("SELECT password FROM user WHERE username = ?");
		prep_stmt->setString(1, userName);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			if (!strcmp(password, res->getString("password").c_str()))
				return true;
			else
				return false;
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

/**
* checkChat11 is used to check two users is in chat type 1-1 or not
* @param id1 id of first user
* @param id2 id of second user
* @return true when 2 users is in chat 1-1
* @return false when 2 users is not in chat 1-1
*/
bool QueryDatabase::checkChat11(const int id1, const int id2) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT count(*) counter FROM group_user g1, group_user g2, group_chat g3 WHERE g1.user_id = ? AND g2.user_id = ? AND g1.group_id = g2.group_id AND g1.group_id = g3.id AND g3.type = 0");
		prep_stmt->setInt(1, id1);
		prep_stmt->setInt(2, id2);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			if (res->getInt("counter") > 0)
				return true;
			else
				return false;
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

/**
* checkGroupName is used to check groupName is used or not
* @param groupName name of group
* @return true group name is not used
* @return false when group name is used
*/
bool QueryDatabase::checkGroupName(const char * groupName) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString groupName(groupName, strlen(groupName));
		prep_stmt = con->prepareStatement("SELECT count(*) counter FROM group_chat where group_name = ?");
		prep_stmt->setString(1, groupName);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			if (res->getInt("counter") == 0)
				return true;
			else
				return false;
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* checkStatus is used to check status of account
* @param userId id of user
* @return true when account is activated
* @return false when account is locked
*/
bool QueryDatabase::checkStatus(const int userId) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT status FROM user where id = ?");
		prep_stmt->setInt(1, userId);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			if (res->getInt("status") == 0)
				return true;
			else
				return false;
		}
		else
			return false;
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* getUserId is used to get id of account by username
* @param userName
* @return id of account has userName
* @return -1 when no account has userName
*/
int QueryDatabase::getUserId(const char * userName) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString userName(userName, strlen(userName));
		prep_stmt = con->prepareStatement("SELECT id FROM user where username = ?");
		prep_stmt->setString(1, userName);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			return res->getInt("id");
		}
		else
			return -1;
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* getIdGroup11 is used to get id of group chat 1-1
* @param userId1
* @param userId2
* @return id of group chat 1-1
* @return -1 when two user is not in chat 1-1
*/
int QueryDatabase::getIdGroup11(const int userId1, const int userId2) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT gc.id groupId11 FROM group_user gu1, group_user gu2, group_chat gc WHERE gu1.user_id = ? AND gu2.user_id = ? AND gu1.group_id = gu2.group_id AND gu1.group_id = gc.id AND gc.type = 0");
		prep_stmt->setInt(1, userId1);
		prep_stmt->setInt(2, userId2);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			return res->getInt("groupId11");
		}
		else
			return -1;
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* getIdGroup11 is used to get id of group chat 1-n
* @param groupName
* @return id of group chat 1-n
* @return -1 when group not exist
*/
int QueryDatabase::getIdGroup1n(const char* groupName) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");
		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString groupName(groupName, strlen(groupName));
		prep_stmt = con->prepareStatement("SELECT id FROM group_chat WHERE group_name = ? AND type = 1");
		prep_stmt->setString(1, groupName);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			return res->getInt("id");
		}
		else
			return -1;
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* getListGroupUser is used to get list group of user joined
* @param userId
* @return all name of group that user joined
*/
char * QueryDatabase::getListGroupOfUser(const int userId) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;
		char result[1000];
		strcpy(result, "");
		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");
		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT gc.group_name FROM group_user gu, group_chat gc WHERE gu.user_id = ? AND gu.group_id = gc.id");
		prep_stmt->setInt(1, userId);
		res = prep_stmt->executeQuery();
		while (res->next()) {
			strcat(result, res->getString("group_name").c_str());
			strcat(result, " ");
		}
		result[strlen(result)] = 0;
		return result;
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

/**
* getUserNameById is used to get username of account by id
* @param userId
* @return username off account that has userId
* @return NULL if userId is not exist in database
*/
char* QueryDatabase::getUsernameById(const int userId) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;
		char result[1000];
		strcpy(result, "");
		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");
		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT username FROM user WHERE id = ?");
		prep_stmt->setInt(1, userId);
		res = prep_stmt->executeQuery();
		if (res->next()) {
			strcpy(result, res->getString("username").c_str());
			return result;
		}
		else
			return NULL;
		
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}
/**
* pushMess is used to push message into database
* @param mess
* @param groupId id of group chat that sender send to
* @param userId id of sender user
* @return no return value
*/
void QueryDatabase::pushMess(char* mess, const int groupId, const int userId) {	
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");
		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString mess(mess, strlen(mess));
		prep_stmt = con->prepareStatement("INSERT INTO chat_history (user_id, message, group_id) VALUE (?, ?, ?)");
		prep_stmt->setInt(1, userId);
		prep_stmt->setString(2, mess);
		prep_stmt->setInt(3, groupId);
		prep_stmt->executeQuery();

		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

void QueryDatabase::getMessGroup(vector<char*> &mess, const int groupId, const int amount) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");
		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT u.username, c.message FROM user u, chat_history c WHERE c.group_id = ? AND c.user_id = u.id LIMIT ?");
		prep_stmt->setInt(1, groupId);
		prep_stmt->setInt(2, amount);
		res = prep_stmt->executeQuery();
		while (res->next()) {
			char result[50];
			strcpy(result, res->getString("username").c_str());
			strcat(result, " ");
			strcat(result, res->getString("message").c_str());
			result[strlen(result)] = 0;
			char* copy = new char[strlen(result) + 1];
			memcpy(copy, result, strlen(result) + 1);
			mess.push_back(copy);
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

void QueryDatabase::getListIdUser(vector <int> &listIdUser, const int groupId) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		///* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");
		///* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		prep_stmt = con->prepareStatement("SELECT user_id FROM group_user WHERE group_id = ?");
		prep_stmt->setInt(1, groupId);
		res = prep_stmt->executeQuery();
		while (res->next()) {
			int id;
			id = res->getInt("user_id");			
			listIdUser.push_back(id);
		}
		delete res;
		delete prep_stmt;
		delete con;
	}
	catch (SQLException &e) {
		cout << "SQLExeption" << endl;
	}
}

// map <int userId, char[] nameOfGroup>

// thang map cuar user id = 1 tham gia trong 1 list cac nhom chat 
// map[1] = list cua 1 danh sach cac nhom chat
// list<int> group;
// group.push(1)
// group.push(2)

// map[1] = group;