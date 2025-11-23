// PackageDAO.h
#ifndef PACKAGEDAO_H
#define PACKAGEDAO_H

#include <vector>
#include <string>
#include <mysql_connection.h>

// Include Model Package
#include "Package.h"

class PackageDAO {
private:
    sql::Connection* con;

public:
    PackageDAO(sql::Connection* conn);

    // CRUD Operations
    void addPackage(const Package& pkg);
    std::vector<Package> getAllPackages();
    Package getPackageById(int id);
    void updatePackage(const Package& pkg);
    void deletePackage(int id);

    // Fitur Tambahan (Opsional tapi berguna)
    // Mencari paket berdasarkan Kurir
    std::vector<Package> getPackagesByKurir(std::string kurirId);
};

#endif