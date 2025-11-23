#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "UserDAO.h"
#include <string>

class AuthService {
private:
    UserDAO* userDAO;
    User* loggedInUser; // Menyimpan sesi user yang sedang login

public:
    AuthService(UserDAO* dao);
    ~AuthService();

    // Mengembalikan true jika login berhasil
    bool login(std::string userId, std::string userName);
    void logout();

    // Cek status
    bool isAuthenticated();
    User* getCurrentUser();
};

#endif