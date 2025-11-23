#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "UserDAO.h"
#include <string>
#include <iostream>

class UserService {
private:
    UserDAO* userDAO; // Dependency Injection

public:
    UserService(UserDAO* dao);

    // Business Logic
    void registerUser(const User& user);
    void showAllUsers();
};

#endif