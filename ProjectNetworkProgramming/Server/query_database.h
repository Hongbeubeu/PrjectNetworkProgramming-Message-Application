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
	QueryDatabase();
	~QueryDatabase();
	void AddUser(const char *, const char *);
	int getUserIdByUserName(const char *);
	void LockAccount(const char *);
	void CreateGroup(const char *, const char *);
	void InsertUserToGroup(int , int);
	bool checkUserName(const char *);
	bool checkPassword(const char *, const char *);
	bool checkChat11(int, int);
	bool checkGroupName(const char*);

};
