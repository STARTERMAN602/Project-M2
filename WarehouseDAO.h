#ifndef WAREHOUSEDAO_H
#define WAREHOUSEDAO_H

#include <vector>
#include <mysql_connection.h>
#include "Warehouse.h"

class WarehouseDAO {
private:
    sql::Connection* con;
public:
    WarehouseDAO(sql::Connection* conn);

    // CRUD
    void addWarehouse(const Warehouse& wh);
    std::vector<Warehouse> getAllWarehouses();
    // Validasi apakah Hub ID ada
    bool isHubExists(int id);
};
#endif