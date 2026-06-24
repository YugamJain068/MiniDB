#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

#include "row.h"

class Table
{
public:
    Table(const std::string& tableName);

    void insert(const Row& row);

    std::vector<Row> selectAll();

private:
    std::string filename;
};

#endif