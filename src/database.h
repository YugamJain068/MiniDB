#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database
{
public:
    bool createTable(const std::string& tableName);
    void selectAll(const std::string& tableName);

private:
    std::vector<std::string> tables;
};

#endif