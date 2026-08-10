#include "table.h"

#include <fstream>
#include <iostream>

using namespace std;

Table::Table(const std::string &tableName, BufferPoolManager &bufferPool)
    : filename("../data/" + tableName + ".tbl"),
      bufferPool(bufferPool) {}

RecordPointer Table::insert(const Row &row)
{
    int pageCount =
        bufferPool.GetPageCount(filename);

    int pageId =
        (pageCount == 0)
            ? bufferPool.AllocatePage(filename)
            : pageCount - 1;

    Page *page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (!page->insertRow(row))
    {
        bufferPool.UnpinPage(filename, pageId);

        pageId =
            bufferPool.AllocatePage(filename);

        page =
            bufferPool.FetchPage(
                filename,
                pageId);

        page->insertRow(row);
    }

    int slotId =
        page->getRowCount() - 1;

    bufferPool.MarkDirty(filename, pageId);

    bufferPool.UnpinPage(filename, pageId);

    return RecordPointer(
        pageId,
        slotId);
}

vector<Row> Table::selectAll()
{
    vector<Row> rows;

    int pageCount =
        bufferPool.GetPageCount(filename);

    for (int pageNumber = 0;
         pageNumber < pageCount;
         pageNumber++)
    {
        Page *page =
            bufferPool.FetchPage(
                filename,
                pageNumber);

        for (int i = 0;
             i < page->getRowCount();
             i++)
        {
            rows.push_back(
                page->getRow(i));
        }

        bufferPool.UnpinPage(filename, pageNumber);
    }

    return rows;
}

Row Table::readRow(int pageId, int slotId)
{
    Page *page =
        bufferPool.FetchPage(
            filename,
            pageId);

    Row row =
        page->getRow(slotId);

    bufferPool.UnpinPage(filename, pageId);

    return row;
}