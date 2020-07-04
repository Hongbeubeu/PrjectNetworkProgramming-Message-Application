#include "stdafx.h"
#include "query_database.h"

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
void QueryDatabase::AddUser(const char * userName, const char * password) {
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
void QueryDatabase::LockAccount(const char * userName) {
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
void QueryDatabase::CreateGroup(const char * groupName, const char * userName){
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;
		ResultSet *res;

		int groupId;
		int userId;
		
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		/* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString groupName(groupName, strlen(groupName));
		SQLString userName(userName, strlen(userName));
		prep_stmt = con->prepareStatement("INSERT INTO group_chat (group_name) VALUES (?)");
		prep_stmt->setString(1, groupName);
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
		InsertUserToGroup(userId, groupId);
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
void QueryDatabase::InsertUserToGroup(int userId, int groupId){
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
bool QueryDatabase::checkChat11(int id1, int id2) {
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


// map <int userId, char[] nameOfGroup>

// thang map cuar user id = 1 tham gia trong 1 list cac nhom chat 
// map[1] = list cua 1 danh sach cac nhom chat
// list<int> group;
// group.push(1)
// group.push(2)

// map[1] = group;