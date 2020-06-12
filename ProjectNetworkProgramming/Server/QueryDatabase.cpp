#include "stdafx.h"
#include "query_database.h"

QueryDatabase::QueryDatabase() {

};

QueryDatabase::~QueryDatabase() {

};


void QueryDatabase::AddUser(const char * username, const char * password, int status) {
	try {
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3307", "root", "123456");

		/* Connect to the MySQL messenger database */
		con->setSchema("messenger");
		SQLString username(username, strlen(username));
		SQLString password(password, strlen(password));
		prep_stmt = con->prepareStatement("INSERT INTO user (username, password, status) VALUES (?, ?, ?)");

		prep_stmt->setString(1, username);
		prep_stmt->setString(2, password);
		prep_stmt->setInt(3, status);
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