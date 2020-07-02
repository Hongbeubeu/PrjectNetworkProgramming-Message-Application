#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace sql;

/**
 * Query to database
 */
class QueryDatabase {
public:
	Driver *driver;
	Connection *con;
	PreparedStatement *prep_stmt;

	QueryDatabase();
	~QueryDatabase();
	void AddUser(const char *, const char *);
	void LockAccount(const char *);
	void CreateGroup(const char *, const char *);
	void InsertUserToGroup(int , int);
	bool checkAccount(const char *, const char *);
	bool checkUserName(const char *);

};
