#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

#include "storage_manager.h"

class Table
{
public:
    Table(const std::string& tableName);

    void insert(const Row& row);

    std::vector<Row> selectAll();

private:
    std::string filename;
    StorageManager storageManager;
};

#endif