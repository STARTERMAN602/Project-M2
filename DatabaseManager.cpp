#include "DatabaseManager.h"

using namespace std;

DatabaseManager::DatabaseManager() {
    driver = nullptr;
    con = nullptr;
}

DatabaseManager::~DatabaseManager() {
    if (con) delete con;
}

void DatabaseManager::connect() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();

        // Port 3307 & Password kamu
        con = driver->connect("tcp://localhost:3307", "root", "Arnoldadra03092006!");

        // Opsional: Set schema/database kalau sudah ada, kalau belum ada baris ini bisa dihapus
        con->setSchema("pengiriman_barang"); 

        // INI PESANNYA
        cout << "------------------------------------------------" << endl;
        cout << "           [SUCCESS] Welcome User.              " << endl;
        cout << "------------------------------------------------" << endl;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Failed to Connect: " << e.what() << endl;
        throw;
    }
}
sql::Connection* DatabaseManager::getConnection() {
    return con;
}