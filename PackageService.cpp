#include "PackageService.h"

// Constructor
PackageService::PackageService(PackageDAO* pDao, UserDAO* uDao) {
    this->packageDAO = pDao;
    this->userDAO = uDao;
}

// 1. Create Package (Frontliner)
void PackageService::sendPackage(const Package& pkg) {
    // Validasi Kurir: Apakah kurir ID valid?
    User kurir = userDAO->getUserById(pkg.kurir_id);

    // Jika kurir ID diisi (tidak kosong/strip), validasi datanya
    if (pkg.kurir_id != "-" && pkg.kurir_id != "") {
        if (kurir.user_id == "") {
            std::cout << "[ERROR] Kurir ID " << pkg.kurir_id << " tidak ditemukan!" << std::endl;
            return;
        }

        if (kurir.user_role != "kurir") {
            std::cout << "[ERROR] User " << kurir.user_name << " bukan seorang Kurir!" << std::endl;
            return;
        }
    }

    // Validasi ID Paket
    if (pkg.receipt_id <= 0) {
        std::cout << "[ERROR] ID Resi tidak valid." << std::endl;
        return;
    }

    // Cek duplikat
    if (packageDAO->getPackageById(pkg.receipt_id).receipt_id != -1) {
        std::cout << "[ERROR] Resi ID " << pkg.receipt_id << " sudah ada di database!" << std::endl;
        return;
    }

    packageDAO->addPackage(pkg);
    std::cout << "[SUCCESS] Paket berhasil diproses ke sistem." << std::endl;
}

// 2. Track Package (General/User)
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

// ==========================================
//          IMPLEMENTASI FITUR BARU
// ==========================================

// 3. Show All Packages (Admin)
void PackageService::showAllPackages() {
    std::vector<Package> allPackages = packageDAO->getAllPackages();

    if (allPackages.empty()) {
        std::cout << "[INFO] Tidak ada data paket." << std::endl;
        return;
    }

    std::cout << "\n===========================================================" << std::endl;
    std::cout << "               LAPORAN SEMUA PAKET (" << allPackages.size() << ")" << std::endl;
    std::cout << "===========================================================" << std::endl;

    for (const auto& p : allPackages) {
        std::cout << "Resi: " << p.receipt_id
            << " | Loc: " << p.current_hub
            << " | Kurir: " << p.kurir_id
            << " | Tujuan: " << p.alamat_tujuan << std::endl;
    }
    std::cout << "===========================================================" << std::endl;
}

// 4. Update Status (Frontliner & Kurir)
void PackageService::updateStatus(int receiptId, std::string lokasiBaru, int hubIdBaru) {
    Package p = packageDAO->getPackageById(receiptId);

    if (p.receipt_id == -1) {
        std::cout << "[ERROR] Resi " << receiptId << " tidak ditemukan." << std::endl;
        return;
    }

    // Update data di memori
    p.current_hub = lokasiBaru;

    // Jika hubIdBaru dikirim 0, berarti kita tidak mau ubah ID hub (cuma ubah status teks)
    if (hubIdBaru != 0) {
        p.hub_id = hubIdBaru;
    }

    // Simpan ke database
    packageDAO->updatePackage(p);
    std::cout << "[SUCCESS] Status paket updated: " << lokasiBaru << std::endl;
}

// 5. Assign Kurir (Frontliner)
void PackageService::assignKurir(int receiptId, std::string kurirId) {
    // Validasi Kurir
    User k = userDAO->getUserById(kurirId);
    if (k.user_id == "" || k.user_role != "kurir") {
        std::cout << "[ERROR] ID Kurir tidak valid." << std::endl;
        return;
    }

    Package p = packageDAO->getPackageById(receiptId);
    if (p.receipt_id == -1) {
        std::cout << "[ERROR] Resi tidak ditemukan." << std::endl;
        return;
    }

    p.kurir_id = kurirId;
    packageDAO->updatePackage(p);

    std::cout << "[SUCCESS] Paket " << receiptId << " diserahkan ke kurir " << k.user_name << std::endl;
}

// 6. Show My Packages (Kurir)
void PackageService::showMyPackages(std::string kurirId) {
    std::vector<Package> all = packageDAO->getAllPackages();
    bool found = false;

    std::cout << "\n=== DAFTAR TUGAS PENGIRIMAN ANDA ===" << std::endl;
    for (const auto& p : all) {
        // Tampilkan paket milik kurir ini yang statusnya BELUM 'DELIVERED'
        if (p.kurir_id == kurirId && p.current_hub != "DELIVERED") {
            std::cout << "-> Resi: " << p.receipt_id
                << " | Barang: " << p.jenis_barang
                << " | Tujuan: " << p.alamat_tujuan << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "[INFO] Tidak ada tugas pengiriman aktif." << std::endl;
    }
}
