#ifndef PACKAGESERVICE_H
#define PACKAGESERVICE_H

#include "PackageDAO.h"
#include "UserDAO.h" // Butuh validasi kurir
#include <iostream>

class PackageService {
private:
    PackageDAO* packageDAO;
    UserDAO* userDAO;

public:
    PackageService(PackageDAO* pDao, UserDAO* uDao);

    void sendPackage(const Package& pkg);
    void trackPackage(int receiptId);
};

#endif