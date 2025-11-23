#include "Menu.h" // Pastikan nama header sesuai (MenuUI.h atau Menu.h)
#include <iostream>
#include <limits>

using namespace std;

MenuUI::MenuUI(UserService* uSvc, AuthService* aSvc, PackageService* pSvc) {
    this->userService = uSvc;
    this->authService = aSvc;
    this->packageService = pSvc;
}

void MenuUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MenuUI::pause() {
    cout << "\nTekan Enter kembali ke menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void MenuUI::run() {
    User* currentUser = authService->getCurrentUser();

    // Routing menu berdasarkan Role
    if (currentUser->user_role == "admin") {
        menuAdmin();
    }
    else if (currentUser->user_role == "frontliner") {
        menuFrontliner();
    }
    else if (currentUser->user_role == "kurir") {
        menuKurir();
    }
    else {
        cout << "Role tidak valid!" << endl;
    }
}

// ==========================================
//           IMPLEMENTASI MENU ADMIN
// ==========================================
void MenuUI::menuAdmin() {
    int choice;
    do {
        clearScreen();
        cout << "=== DASHBOARD ADMIN (" << authService->getCurrentUser()->user_name << ") ===" << endl;
        cout << "--- Manajemen User ---" << endl;
        cout << "1. Lihat Semua User (Read)" << endl;
        cout << "2. Tambah User Baru (Create)" << endl;
        cout << "3. Edit Data User (Update)" << endl;
        cout << "4. Hapus User (Delete)" << endl;

        cout << "\n--- Laporan ---" << endl;
        cout << "5. Lihat Semua Paket (Global Tracking)" << endl;

        cout << "\n0. Logout" << endl;
        cout << ">> "; cin >> choice;

        switch (choice) {
        case 1: {
            userService->showAllUsers();
            pause();
            break;
        }
        case 2: {
            User u;
            cout << "\n[TAMBAH USER]" << endl;
            cout << "ID (3 digit): "; cin >> u.user_id;
            cout << "Nama        : "; cin.ignore(); getline(cin, u.user_name);
            cout << "Role (admin/kurir/frontliner): "; cin >> u.user_role;

            userService->registerUser(u);
            pause();
            break;
        }
        case 3: {
            User u;
            cout << "\n[EDIT USER]" << endl;
            cout << "Masukkan ID User yang mau diedit: "; cin >> u.user_id;
            cout << "Nama Baru : "; cin.ignore(); getline(cin, u.user_name);
            cout << "Role Baru : "; cin >> u.user_role;

            userService->editUser(u);
            pause();
            break;
        }
        case 4: {
            string targetId;
            cout << "\n[HAPUS USER]" << endl;
            cout << "Masukkan ID User yang mau dihapus: "; cin >> targetId;

            if (targetId == authService->getCurrentUser()->user_id) {
                cout << "[ERROR] Anda tidak bisa menghapus akun anda sendiri saat login!" << endl;
            }
            else {
                char confirm;
                cout << "Yakin hapus " << targetId << "? (y/n): "; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    userService->removeUser(targetId);
                }
            }
            pause();
            break;
        }
        case 5: {
            // FITUR ADMIN: READ ALL PACKAGES
            // Memanggil fungsi yang sudah kita tambahkan di PackageService
            packageService->showAllPackages();
            pause();
            break;
        }
        case 0:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
            pause();
        }
    } while (choice != 0);
}

// ==========================================
//         IMPLEMENTASI MENU FRONTLINER
// ==========================================
void MenuUI::menuFrontliner() {
    int choice;
    do {
        clearScreen();
        cout << "=== DASHBOARD FRONTLINER (" << authService->getCurrentUser()->user_name << ") ===" << endl;
        cout << "1. Input Paket Baru (Create)" << endl;
        cout << "2. Update Lokasi Paket (Update)" << endl;
        cout << "3. Assign Kurir ke Paket (Update)" << endl;
        cout << "4. Cek Resi / Tracking (Read)" << endl;
        cout << "5. Logout" << endl;
        cout << ">> "; cin >> choice;

        switch (choice) {
        case 1: {
            // CREATE: Input Paket Lengkap
            Package p;
            cout << "\n[INPUT PAKET]" << endl;
            cout << "Resi ID (Angka): "; cin >> p.receipt_id;
            cout << "Hub ID Saat ini: "; cin >> p.hub_id;
            cout << "ID Kurir Awal  : "; cin >> p.kurir_id; // Isi "-" jika belum ada kurir

            cin.ignore(); // Membersihkan buffer sebelum getline

            cout << "Lokasi Kota    : "; getline(cin, p.current_hub);
            p.hub_asal = p.current_hub; // Asal diset sama dengan lokasi awal
            cout << "Kota Tujuan    : "; getline(cin, p.hub_akhir);
            cout << "Jenis Barang   : "; getline(cin, p.jenis_barang);
            cout << "Alamat Lengkap : "; getline(cin, p.alamat_tujuan);

            packageService->sendPackage(p);
            pause();
            break;
        }
        case 2: {
            // UPDATE: Lokasi Paket (Scan Barang Masuk Gudang)
            int resi;
            string lokasiBaru;
            int hubIdBaru;

            cout << "\n[UPDATE LOKASI]" << endl;
            cout << "Resi ID: "; cin >> resi;
            cout << "Hub ID Baru: "; cin >> hubIdBaru;
            cin.ignore();
            cout << "Lokasi Baru (Kota): "; getline(cin, lokasiBaru);

            packageService->updateStatus(resi, lokasiBaru, hubIdBaru);
            pause();
            break;
        }
        case 3: {
            // UPDATE: Assign Kurir
            int resi;
            string idKurir;
            cout << "\n[ASSIGN KURIR]" << endl;
            cout << "Resi ID: "; cin >> resi;
            cout << "ID Kurir Pengantar: "; cin >> idKurir;

            packageService->assignKurir(resi, idKurir);
            pause();
            break;
        }
        case 4: {
            // READ: Tracking
            int resi;
            cout << "Masukkan No Resi: "; cin >> resi;
            packageService->trackPackage(resi);
            pause();
            break;
        }
        case 5:
            return; // Logout
        default:
            cout << "Pilihan tidak valid." << endl;
            pause();
        }
    } while (choice != 5);
}

// ==========================================
//           IMPLEMENTASI MENU KURIR
// ==========================================
void MenuUI::menuKurir() {
    // Ambil ID Kurir yang sedang login
    string myId = authService->getCurrentUser()->user_id;
    int choice;

    do {
        clearScreen();
        cout << "=== DASHBOARD KURIR (" << authService->getCurrentUser()->user_name << ") ===" << endl;
        cout << "1. Lihat Tugas Saya (Read)" << endl;
        cout << "2. Update Paket Sampai/Delivered (Update)" << endl;
        cout << "3. Cari Paket Umum (Read)" << endl;
        cout << "4. Logout" << endl;
        cout << ">> "; cin >> choice;

        switch (choice) {
        case 1: {
            // READ: Menampilkan paket khusus kurir ini
            packageService->showMyPackages(myId);
            pause();
            break;
        }
        case 2: {
            // UPDATE: Paket Sampai
            int resi;
            cout << "\n[PAKET SAMPAI]" << endl;
            cout << "Masukkan Resi yang sudah diantar: "; cin >> resi;

            // Update lokasi jadi "DELIVERED", Hub ID 0 (karena sudah keluar sistem logistik)
            packageService->updateStatus(resi, "DELIVERED", 0);
            pause();
            break;
        }
        case 3: {
            // READ: General Tracking
            int resi;
            cout << "Cek Resi: "; cin >> resi;
            packageService->trackPackage(resi);
            pause();
            break;
        }
        case 4:
            return; // Logout
        default:
            cout << "Pilihan tidak valid." << endl;
            pause();
        }
    } while (choice != 4);
}
