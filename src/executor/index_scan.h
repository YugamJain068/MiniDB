#ifndef INDEX_SCAN_H
#define INDEX_SCAN_H

#include "execution_operator.h"

#include "../storage/table.h"
#include "../bptree/b_plus_tree.h"

class IndexScan : public ExecutionOperator
{
private:

    Table& table;
    BPlusTree& index;

    int searchKey;

    bool opened;
    bool returned;

public:

    IndexScan(
        Table& table,
        BPlusTree& index,
        int searchKey);

    bool open() override;

    bool next(Row& row) override;

    void close() override;
};

#endif