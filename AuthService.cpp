#include "AuthService.h"
#include <iostream>

AuthService::AuthService(UserDAO* dao) {
    this->userDAO = dao;
    this->loggedInUser = nullptr;
}

AuthService::~AuthService() {
    // loggedInUser hanya pointer, datanya dikelola manual/stack, tidak perlu delete
    loggedInUser = nullptr;
}

bool AuthService::login(std::string userId, std::string userName) {
    // 1. Cari user di DB
    User u = userDAO->getUserById(userId);

    // 2. Cek apakah user ada?
    if (u.user_id == "") {
        std::cout << "[LOGIN FAILED] User ID tidak ditemukan." << std::endl;
        return false;
    }

    // 3. Cek apakah Nama cocok (Simulasi Password)
    if (u.user_name == userName) {
        // Alokasikan memori baru untuk session
        if (loggedInUser) delete loggedInUser;
        loggedInUser = new User(u);

        std::cout << "[LOGIN SUCCESS] Selamat datang, " << u.user_name << " (" << u.user_role << ")!" << std::endl;
        return true;
    }
    else {
        std::cout << "[LOGIN FAILED] Nama salah/tidak cocok." << std::endl;
        return false;
    }
}

void AuthService::logout() {
    if (loggedInUser) {
        std::cout << "[LOGOUT] Sampai jumpa " << loggedInUser->user_name << std::endl;
        delete loggedInUser;
        loggedInUser = nullptr;
    }
}

bool AuthService::isAuthenticated() {
    return loggedInUser != nullptr;
}

User* AuthService::getCurrentUser() {
    return loggedInUser;
}