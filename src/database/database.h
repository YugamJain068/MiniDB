#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "../models/row.h"
#include "../buffer/buffer_pool_manager.h"
#include "../bptree/b_plus_tree.h"
#include <unordered_map>
#include "../storage/table.h"

class Database
{
public:
    Database();

    bool createTable(const std::string &tableName);
    std::vector<Row> selectAll(const std::string &tableName);
    Row selectByPointer(const std::string &tableName, const RecordPointer &ptr);
    bool tableExists(const std::string &tableName);
    BufferPoolManager &getBufferPool();

    BPlusTree &getIndex();

    Table& getTable(const std::string& tableName);

private:
    std::unordered_map<std::string, Table> tables;
    BufferPoolManager bufferPool;

    BPlusTree index;
};

#endif