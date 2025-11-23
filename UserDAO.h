#ifndef USERDAO_H
#define USERDAO_H

#include <vector>
#include <string>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#include "User.h"

using namespace std;

class UserDAO {
private:
    sql::Connection* con;

public:
    UserDAO(sql::Connection* conn);

    ~UserDAO();

    void addUser(const User& user);
    std::vector<User> getAllUsers();
    User getUserById(string id);
    void updateUser(const User& user);
    void deleteUser(string id);
};

#endif