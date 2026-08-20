#include "index_scan.h"

IndexScan::IndexScan(
    Table& table,
    BPlusTree& index,
    int searchKey)
    : table(table),
      index(index),
      searchKey(searchKey),
      opened(false),
      returned(false)
{

}bool IndexScan::open()
{
    opened = true;
    returned = false;

    return true;
}

bool IndexScan::next(Row& row)
{
    if (!opened || returned)
        return false;

    RecordPointer ptr =
        index.search(searchKey);

    if (ptr.pageId == -1)
        return false;

    row =
        table.readRow(
            ptr.pageId,
            ptr.slotId);

    returned = true;

    return true;
}

void IndexScan::close()
{
    opened = false;
    returned = false;
}