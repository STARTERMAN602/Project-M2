#include "PackageService.h"

PackageService::PackageService(PackageDAO* pDao, UserDAO* uDao) {
    this->packageDAO = pDao;
    this->userDAO = uDao;
}

void PackageService::sendPackage(const Package& pkg) {
    // 1. Validasi Kurir: Apakah kurir ID valid?
    User kurir = userDAO->getUserById(pkg.kurir_id);
    if (kurir.user_id == "") {
        std::cout << "[ERROR] Kurir ID " << pkg.kurir_id << " tidak ditemukan!" << std::endl;
        return;
    }

    if (kurir.user_role != "kurir") {
        std::cout << "[ERROR] User " << kurir.user_name << " bukan seorang Kurir!" << std::endl;
        return;
    }

    // 2. Validasi ID Paket (Harus positif)
    if (pkg.receipt_id <= 0) {
        std::cout << "[ERROR] ID Resi tidak valid." << std::endl;
        return;
    }

    // 3. Simpan
    packageDAO->addPackage(pkg);
    std::cout << "[SERVICE] Paket berhasil diproses ke sistem." << std::endl;
}

void PackageService::trackPackage(int receiptId) {
    Package p = packageDAO->getPackageById(receiptId);

    if (p.receipt_id != -1) {
        std::cout << "\n--- TRACKING RESULT ---" << std::endl;
        std::cout << "Resi   : " << p.receipt_id << std::endl;
        std::cout << "Barang : " << p.jenis_barang << std::endl;
        std::cout << "Posisi : " << p.current_hub << std::endl;
        std::cout << "Tujuan : " << p.alamat_tujuan << std::endl;
        std::cout << "Kurir  : " << p.kurir_id << std::endl;
    }
    else {
        std::cout << "[INFO] Paket dengan Resi " << receiptId << " tidak ditemukan." << std::endl;
    }
}