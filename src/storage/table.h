#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

#include "storage_manager.h"
#include "../storage/record_pointer.h"
#include "../buffer/buffer_pool_manager.h"

class Table
{
public:
    Table(const std::string& tableName, BufferPoolManager& bufferPool);

    RecordPointer insert(const Row& row);

    std::vector<Row> selectAll();
    Row readRow(int pageId, int slotId);
    const std::string& getFilename() const;

private:
    std::string filename;
    BufferPoolManager& bufferPool;
};

#endif