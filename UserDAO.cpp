#include "UserDAO.h"

UserDAO::UserDAO(sql::Connection* conn) {
    this->con = conn;
}

UserDAO::~UserDAO() {
    // Koneksi tidak di-delete di sini karena milik DatabaseManager
}

// 1. ADD USER
void UserDAO::addUser(const User& user) {
    try {
        // Query INSERT ke tabel `user`
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "INSERT INTO user (user_id, user_name, user_role) VALUES (?, ?, ?)"
        );

        pstmt->setString(1, user.user_id);
        pstmt->setString(2, user.user_name);
        pstmt->setString(3, user.user_role);

        pstmt->executeUpdate();

        cout << "[INFO] Berhasil menambah user: " << user.user_name << endl;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Gagal Insert: " << e.what() << endl;
    }
}

// 2. GET ALL USERS
vector<User> UserDAO::getAllUsers() {
    vector<User> listUser;
    try {
        sql::Statement* stmt = con->createStatement();
        // SELECT dari tabel `user`
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM user");

        while (res->next()) {
            User u;
            // Ambil data sesuai nama kolom di SQL Dump
            u.user_id = res->getString("user_id");
            u.user_name = res->getString("user_name");
            u.user_role = res->getString("user_role");

            listUser.push_back(u);
        }
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Gagal Load Data: " << e.what() << endl;
    }
    return listUser;
}

// 3. GET USER BY ID
User UserDAO::getUserById(string id) {
    User u;
    u.user_id = ""; // Penanda kalau kosong

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT * FROM user WHERE user_id = ?"
        );
        pstmt->setString(1, id);

        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) {
            u.user_id = res->getString("user_id");
            u.user_name = res->getString("user_name");
            u.user_role = res->getString("user_role");
        }

        delete res;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Gagal cari user: " << e.what() << endl;
    }
    return u;
}

// 4. UPDATE USER
void UserDAO::updateUser(const User& user) {
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE user SET user_name = ?, user_role = ? WHERE user_id = ?"
        );

        pstmt->setString(1, user.user_name);
        pstmt->setString(2, user.user_role);
        pstmt->setString(3, user.user_id);

        int rows = pstmt->executeUpdate();

        if (rows > 0) cout << "[INFO] Update berhasil untuk ID: " << user.user_id << endl;
        else cout << "[INFO] User ID tidak ditemukan." << endl;

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Gagal Update: " << e.what() << endl;
    }
}

// 5. DELETE USER
void UserDAO::deleteUser(string id) {
    try {
        // PERINGATAN: Di database kamu ada Foreign Key!
        // Kalau user ini (misal k01) sudah ada di tabel `paket`, dia TIDAK BISA dihapus
        // kecuali data di tabel `paket` dihapus dulu.

        sql::PreparedStatement* pstmt = con->prepareStatement(
            "DELETE FROM user WHERE user_id = ?"
        );

        pstmt->setString(1, id);

        int rows = pstmt->executeUpdate();

        if (rows > 0) cout << "[INFO] Hapus berhasil untuk ID: " << id << endl;
        else cout << "[INFO] ID tidak ditemukan." << endl;

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        // Menangkap error Foreign Key Constraint
        cerr << "[ERROR] Gagal Hapus: " << e.what() << endl;
        cerr << "        (Kemungkinan user ini sedang dipakai di tabel Paket atau Hub)" << endl;
    }
}