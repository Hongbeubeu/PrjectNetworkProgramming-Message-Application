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
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

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
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

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
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}