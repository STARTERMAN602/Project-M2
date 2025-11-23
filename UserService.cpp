#include "UserService.h"

UserService::UserService(UserDAO* dao) {
    this->userDAO = dao;
}

void UserService::registerUser(const User& user) {
    // 1. Validasi: ID tidak boleh kosong
    if (user.user_id.empty() || user.user_name.empty()) {
        std::cout << "[WARNING] ID dan Nama tidak boleh kosong!" << std::endl;
        return;
    }

    // 2. Validasi: Cek apakah ID sudah dipakai?
    User existing = userDAO->getUserById(user.user_id);
    if (existing.user_id != "") {
        std::cout << "[WARNING] User ID " << user.user_id << " sudah terdaftar!" << std::endl;
        return;
    }

    // 3. Jika aman, panggil DAO
    userDAO->addUser(user);
}

void UserService::showAllUsers() {
    std::vector<User> users = userDAO->getAllUsers();
    std::cout << "\n=== DAFTAR USER (" << users.size() << ") ===" << std::endl;
    for (const auto& u : users) {
        std::cout << "- " << u.user_id << " | " << u.user_name
            << " (" << u.user_role << ")" << std::endl;
    }
}