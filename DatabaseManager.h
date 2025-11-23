#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <iostream>

class DatabaseManager {
private:
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

public:
    DatabaseManager();
    ~DatabaseManager();
    void connect();

    sql::Connection* getConnection();
};

#endif