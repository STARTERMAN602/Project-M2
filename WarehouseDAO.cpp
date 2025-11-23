#include "WarehouseDAO.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>

using namespace std;

WarehouseDAO::WarehouseDAO(sql::Connection* conn) {
    this->con = conn;
}

void WarehouseDAO::addWarehouse(const Warehouse& wh) {
    try {
        // Ingat: Tabel di database namanya 'hub'
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "INSERT INTO hub (hub_id, hub_location, frontliner_id) VALUES (?, ?, ?)"
        );
        pstmt->setInt(1, wh.hub_id);
        pstmt->setString(2, wh.hub_location);
        pstmt->setString(3, wh.frontliner_id);

        pstmt->executeUpdate();
        cout << "[SUCCESS] Hub " << wh.hub_location << " added." << endl;
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        cerr << "[ERROR] Insert Hub Failed: " << e.what() << endl;
    }
}

std::vector<Warehouse> WarehouseDAO::getAllWarehouses() {
    std::vector<Warehouse> list;
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM hub");

        while (res->next()) {
            Warehouse w;
            w.hub_id = res->getInt("hub_id");
            w.hub_location = res->getString("hub_location");
            w.frontliner_id = res->getString("frontliner_id");
            list.push_back(w);
        }
        delete res; delete stmt;
    }
    catch (sql::SQLException& e) { /* Handle error */ }
    return list;
}