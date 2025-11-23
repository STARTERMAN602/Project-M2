// PackageDAO.cpp
#include "PackageDAO.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <iostream>

using namespace std;

PackageDAO::PackageDAO(sql::Connection* conn) {
    this->con = conn;
}

// 1. CREATE (Tambah Paket)
void PackageDAO::addPackage(const Package& pkg) {
    try {
        // Query Insert ke tabel 'paket'
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "INSERT INTO paket (receipt_id, hub_id, kurir_id, current_hub, hub_asal, hub_akhir, jenis_barang, alamat_tujuan) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
        );

        pstmt->setInt(1, pkg.receipt_id);
        pstmt->setInt(2, pkg.hub_id);
        pstmt->setString(3, pkg.kurir_id);
        pstmt->setString(4, pkg.current_hub);
        pstmt->setString(5, pkg.hub_asal);
        pstmt->setString(6, pkg.hub_akhir);
        pstmt->setString(7, pkg.jenis_barang);
        pstmt->setString(8, pkg.alamat_tujuan);

        pstmt->executeUpdate();
        cout << "[SUCCESS] Paket ID " << pkg.receipt_id << " berhasil dibuat." << endl;

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Gagal tambah paket: " << e.what() << endl;
        // Tips: Error sering terjadi kalau kurir_id atau hub_id tidak ada di tabel user/hub
    }
}

// 2. READ ALL (Ambil Semua Paket)
vector<Package> PackageDAO::getAllPackages() {
    vector<Package> list;
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM paket");

        while (res->next()) {
            Package p;
            p.receipt_id = res->getInt("receipt_id");
            p.hub_id = res->getInt("hub_id");
            p.kurir_id = res->getString("kurir_id");
            p.current_hub = res->getString("current_hub");
            p.hub_asal = res->getString("hub_asal");
            p.hub_akhir = res->getString("hub_akhir");
            p.jenis_barang = res->getString("jenis_barang");
            p.alamat_tujuan = res->getString("alamat_tujuan");

            list.push_back(p);
        }
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Gagal load paket: " << e.what() << endl;
    }
    return list;
}

// 3. READ ONE (Ambil 1 Paket by ID)
Package PackageDAO::getPackageById(int id) {
    Package p;
    p.receipt_id = -1; // Penanda kalau tidak ketemu (karena ID paket int)

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM paket WHERE receipt_id = ?");
        pstmt->setInt(1, id);

        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) {
            p.receipt_id = res->getInt("receipt_id");
            p.hub_id = res->getInt("hub_id");
            p.kurir_id = res->getString("kurir_id");
            p.current_hub = res->getString("current_hub");
            p.hub_asal = res->getString("hub_asal");
            p.hub_akhir = res->getString("hub_akhir");
            p.jenis_barang = res->getString("jenis_barang");
            p.alamat_tujuan = res->getString("alamat_tujuan");
        }
        delete res;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Cari paket gagal: " << e.what() << endl;
    }
    return p;
}

// 4. UPDATE (Update Data Paket)
void PackageDAO::updatePackage(const Package& pkg) {
    try {
        // Biasanya yang diupdate adalah Lokasi (current_hub) dan Kurir
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE paket SET hub_id=?, kurir_id=?, current_hub=?, alamat_tujuan=? WHERE receipt_id=?"
        );

        pstmt->setInt(1, pkg.hub_id);
        pstmt->setString(2, pkg.kurir_id);
        pstmt->setString(3, pkg.current_hub);
        pstmt->setString(4, pkg.alamat_tujuan);
        pstmt->setInt(5, pkg.receipt_id); // WHERE condition

        int row = pstmt->executeUpdate();
        if (row > 0) cout << "[SUCCESS] Paket ID " << pkg.receipt_id << " berhasil diupdate." << endl;
        else cout << "[INFO] Paket ID tidak ditemukan." << endl;

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Update gagal: " << e.what() << endl;
    }
}

// 5. DELETE (Hapus Paket)
void PackageDAO::deletePackage(int id) {
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM paket WHERE receipt_id = ?");
        pstmt->setInt(1, id);

        int row = pstmt->executeUpdate();
        if (row > 0) cout << "[SUCCESS] Paket ID " << id << " berhasil dihapus." << endl;
        else cout << "[INFO] Paket ID tidak ditemukan." << endl;

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Hapus gagal: " << e.what() << endl;
    }
}