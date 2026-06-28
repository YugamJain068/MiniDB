#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>
#include <string>

#include "../models/row.h"

class StorageManager
{
public:

    void writeRow(
        const std::string& filename,
        const Row& row);

    std::vector<Row> readRows(
        const std::string& filename);
};

#endif