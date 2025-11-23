#ifndef PACKAGESERVICE_H
#define PACKAGESERVICE_H

#include "PackageDAO.h"
#include "UserDAO.h"
#include <iostream>
#include <vector>
#include <string>

class PackageService {
private:
    PackageDAO* packageDAO;
    UserDAO* userDAO;

public:
    PackageService(PackageDAO* pDao, UserDAO* uDao);

    // Fitur Lama
    void sendPackage(const Package& pkg);
    void trackPackage(int receiptId);

    // === FITUR BARU (UNTUK ADMIN, FRONTLINER, KURIR) ===

    // 1. Admin: Lihat semua paket
    void showAllPackages();

    // 2. Frontliner/Kurir: Update status/lokasi paket
    void updateStatus(int receiptId, std::string lokasiBaru, int hubIdBaru);

    // 3. Frontliner: Pilih kurir untuk paket
    void assignKurir(int receiptId, std::string kurirId);

    // 4. Kurir: Lihat tugas pengiriman sendiri
    void showMyPackages(std::string kurirId);
};

#endif
