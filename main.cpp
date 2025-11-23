#include <iostream>
#include <exception>

// ==========================================
// 1. INCLUDE INFRASTRUCTURE
// ==========================================
#include "DatabaseManager.h"

// ==========================================
// 2. INCLUDE DAOs (Data Access Layer)
// ==========================================
#include "UserDAO.h"
#include "PackageDAO.h"
// #include "WarehouseDAO.h" // (Opsional: Aktifkan jika sudah dibuat)

// ==========================================
// 3. INCLUDE SERVICES (Business Logic Layer)
// ==========================================
#include "UserService.h"
#include "AuthService.h"
#include "PackageService.h"

// ==========================================
// 4. INCLUDE UI (Presentation Layer)
// ==========================================
#include "Login.h"
#include "Menu.h"

using namespace std;

int main() {
    // ---------------------------------------------------------
    // TAHAP 1: INISIALISASI DATABASE
    // ---------------------------------------------------------
    cout << "[SYSTEM] Starting Application..." << endl;

    DatabaseManager dbManager;
    try {
        dbManager.connect();
    }
    catch (sql::SQLException& e) {
        cerr << "[FATAL ERROR] Database Connection Failed: " << e.what() << endl;
        return 1; // Stop aplikasi jika DB gagal
    }
    catch (exception& e) {
        cerr << "[FATAL ERROR] System Error: " << e.what() << endl;
        return 1;
    }

    // Ambil koneksi mentah untuk disebar ke DAO
    sql::Connection* conn = dbManager.getConnection();


    // ---------------------------------------------------------
    // TAHAP 2: DEPENDENCY INJECTION (WIRING)
    // ---------------------------------------------------------
    // Kita merakit aplikasi dari layer paling bawah ke atas

    // A. Setup DAO (Butuh Koneksi DB)
    UserDAO userDAO(conn);
    PackageDAO packageDAO(conn);

    // B. Setup Service (Butuh DAO)
    UserService userService(&userDAO);
    AuthService authService(&userDAO);
    PackageService packageService(&packageDAO, &userDAO);

    // C. Setup UI (Butuh Service)
    LoginUI loginUI(&authService);
    MenuUI menuUI(&userService, &authService, &packageService);


    // ---------------------------------------------------------
    // TAHAP 3: APPLICATION LOOP (MAIN FLOW)
    // ---------------------------------------------------------
    // Logika: Login -> Menu -> Logout -> Ulangi (atau Exit)

    while (true) {
        // 1. Tampilkan Layar Login
        bool isLoginSuccess = loginUI.showLoginScreen();

        if (isLoginSuccess) {
            // 2. Jika Login Berhasil, Masuk ke Menu Utama
            //    (MenuUI akan mendeteksi role user secara otomatis)
            menuUI.run();

            // 3. Jika user memilih "Logout" di dalam menuUI,
            //    Code akan lanjut ke sini. Kita bersihkan sesi.
            authService.logout();

            cout << "\n[SYSTEM] Session ended. Redirecting to login..." << endl;
            // Loop akan kembali ke atas (Login Screen)
        }
        else {
            // 4. Jika showLoginScreen mengembalikan false (User ketik 'exit')
            break; // Keluar dari while loop
        }
    }

    // ---------------------------------------------------------
    // TAHAP 4: CLEANUP & EXIT
    // ---------------------------------------------------------
    cout << "========================================" << endl;
    cout << "   APLIKASI DITUTUP. TERIMA KASIH.      " << endl;
    cout << "========================================" << endl;

    // Destructor DatabaseManager otomatis dipanggil di sini untuk menutup koneksi.
    return 0;
}