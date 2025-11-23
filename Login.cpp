#include "Login.h"
#include <iostream>
#include <limits>

using namespace std;

LoginUI::LoginUI(AuthService* aSvc) {
    this->authService = aSvc;
}

bool LoginUI::showLoginScreen() {
    string userId, userName;

    // Clear Screen sederhana
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout << "========================================" << endl;
    cout << "          LOGIN SYSTEM LOGISTIK         " << endl;
    cout << "========================================" << endl;
    cout << "(Ketik 'exit' pada User ID untuk keluar)" << endl;

    cout << "\nUser ID  : ";
    if (cin.peek() == '\n') cin.ignore();
    getline(cin, userId);

    if (userId == "exit") return false; // Sinyal untuk tutup aplikasi

    cout << "Username : ";
    getline(cin, userName);

    if (authService->login(userId, userName)) {
        cout << "\nLogin Berhasil! Tekan Enter...";
        cin.get();
        return true; // Sinyal lanjut ke Menu
    }
    else {
        cout << "\nLogin Gagal! Tekan Enter untuk coba lagi...";
        cin.get();
        return showLoginScreen(); // Rekursif: Panggil diri sendiri lagi
    }
}