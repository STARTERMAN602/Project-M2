#include "Menu.h"
#include <iostream>
#include <limits>

using namespace std;

MenuUI::MenuUI(UserService* uSvc, AuthService* aSvc, PackageService* pSvc) {
    this->userService = uSvc;
    this->authService = aSvc;
    this->packageService = pSvc;
}

void MenuUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MenuUI::pause() {
    cout << "\nTekan Enter kembali ke menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void MenuUI::run() {
    User* currentUser = authService->getCurrentUser();

    // Routing menu berdasarkan Role
    if (currentUser->user_role == "admin") {
        menuAdmin();
    }
    else if (currentUser->user_role == "frontliner") {
        menuFrontliner();
    }
    else if (currentUser->user_role == "kurir") {
        menuKurir();
    }
    else {
        cout << "Role tidak valid!" << endl;
    }
}

// --- IMPLEMENTASI MENU SPESIFIK ---

void MenuUI::menuAdmin() {
    int choice;
    do {
        clearScreen();
        cout << "=== DASHBOARD ADMIN (" << authService->getCurrentUser()->user_name << ") ===" << endl;
        cout << "1. List User" << endl;
        cout << "2. Register User Baru" << endl;
        cout << "3. Logout" << endl;
        cout << ">> "; cin >> choice;

        if (choice == 1) {
            userService->showAllUsers();
            pause();
        }
        else if (choice == 2) {
            User u;
            cout << "ID: "; cin >> u.user_id;
            cout << "Nama: "; cin.ignore(); getline(cin, u.user_name);
            cout << "Role: "; cin >> u.user_role;
            userService->registerUser(u);
            pause();
        }
    } while (choice != 3);
}

void MenuUI::menuFrontliner() {
    int choice;
    do {
        clearScreen();
        cout << "=== DASHBOARD FRONTLINER ===" << endl;
        cout << "1. Input Paket" << endl;
        cout << "2. Logout" << endl;
        cout << ">> "; cin >> choice;

        if (choice == 1) {
            Package p;
            cout << "Resi ID: "; cin >> p.receipt_id;
            cout << "Hub ID : "; cin >> p.hub_id;
            cout << "Kurir  : "; cin >> p.kurir_id;
            // ... input sisa data ...
            packageService->sendPackage(p);
            pause();
        }
    } while (choice != 2);
}

void MenuUI::menuKurir() {
    int choice;
    do {
        clearScreen();
        cout << "=== DASHBOARD KURIR ===" << endl;
        cout << "1. Cek Paket Saya" << endl;
        cout << "2. Logout" << endl;
        cout << ">> "; cin >> choice;

        if (choice == 1) {
            int resi;
            cout << "Cek Resi: "; cin >> resi;
            packageService->trackPackage(resi);
            pause();
        }
    } while (choice != 2);
}