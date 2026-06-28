#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "../models/row.h"

class Database
{
public:
    Database();

    bool createTable(const std::string& tableName);
    std::vector<Row> selectAll(const std::string& tableName);
    bool tableExists(const std::string& tableName);

private:
    std::vector<std::string> tables;
};

#endif