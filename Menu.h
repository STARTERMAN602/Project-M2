#ifndef MENU_H
#define MENU_H

#include "UserService.h"
#include "AuthService.h"
#include "PackageService.h"

class MenuUI {
private:
    UserService* userService;
    AuthService* authService;
    PackageService* packageService;

    void clearScreen();
    void pause();

    // Menu Internal
    void menuAdmin();
    void menuFrontliner();
    void menuKurir();

public:
    MenuUI(UserService* uSvc, AuthService* aSvc, PackageService* pSvc);

    // Entry point utama menu
    void run();
};

#endif