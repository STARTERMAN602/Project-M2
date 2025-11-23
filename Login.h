#ifndef LOGIN_H
#define LOGIN_H

#include "AuthService.h"

class LoginUI {
private:
    AuthService* authService;

public:
    LoginUI(AuthService* aSvc);

    // Return true jika login berhasil, false jika user ingin exit aplikasi
    bool showLoginScreen();
};

#endif