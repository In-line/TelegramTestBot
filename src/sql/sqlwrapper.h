#ifndef SQLWRAPPER_H
#define SQLWRAPPER_H

#include <pqxx/pqxx>

#include <memory>

#include "../chat/botuser.h"
#include "../chat/virtualroom.h"

#include "../helper/typedefs.h"

#include "../helper/useractions.h"

class SqlWrapper
{
private:
	pqxx::connection db_connection_;
	std::shared_ptr<UsersCollection> users_;
	std::shared_ptr<UserActions> actions_;

	void loadDatabase();
public:
	SqlWrapper(std::shared_ptr<UsersCollection> &users, std::shared_ptr<UserActions> &actions);
	~SqlWrapper();
	void updateUser(std::shared_ptr<BotUser> &user);

	void insertUser(std::shared_ptr<BotUser> &user);

};

#endif // SQLWRAPPER_H
