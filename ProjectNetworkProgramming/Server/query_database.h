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
	void AddUser(const char *, const char *,int);
	QueryDatabase();
	~QueryDatabase();

};
