#ifndef SEQUENTIAL_SCAN_H
#define SEQUENTIAL_SCAN_H

#include "execution_operator.h"

#include "../storage/table.h"
#include "../buffer/buffer_pool_manager.h"

#include <string>

class SequentialScan : public ExecutionOperator
{
private:

    Table& table;

    int currentPage;

    int currentSlot;

    int pageCount;

    bool opened;

public:

    SequentialScan(
        Table& table,
        BufferPoolManager& bufferPool);

    bool open() override;

    bool next(Row& row) override;

    void close() override;
    

private:

    BufferPoolManager& bufferPool;
};

#endif