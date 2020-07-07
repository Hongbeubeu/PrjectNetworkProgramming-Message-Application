#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <vector>

using namespace sql;
using namespace std;

/**
 * Query to database
 */
class QueryDatabase {
public:
	QueryDatabase();
	~QueryDatabase();
	void addUser(const char * userName, const char * password);
	int getUserIdByUserName(const char * userName);
	void lockAccount(const char * userName);
	void createGroup(const char * groupName, const char * userName,const int type);
	void insertUserToGroup(const int userId, const int groupId);
	bool checkUserName(const char * userName);
	bool checkPassword(const char * userName, const char * password);
	bool checkChat11(const int id1, const int id2);
	bool checkGroupName(const char * groupName);
	bool checkStatus(const int userId);
	int getUserId(const char * userName);
	int getIdGroup11(const int userId1, const int userId2);
	int getIdGroup1n(const char * groupName);
	char* getListGroupOfUser(const int userId);
	char* getUsernameById(const int userId);
	void pushMess(char* mess, const int groupId, const int user);
	void getMessGroup(vector <char*> &mess, const int groupId, const int amount);
	void getListIdUser(vector <int> &listIdUser, const int groupId);
};
