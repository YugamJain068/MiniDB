#include "table.h"

#include <fstream>
#include <iostream>

using namespace std;

Table::Table(const string &tableName)
{
    filename = "../data/" + tableName + ".tbl";
}

void Table::insert(const Row &row)
{
    int pageCount = storageManager.getPageCount(filename);

    if (pageCount == 0)
    {
        storageManager.allocatePage(filename);
        pageCount = 1;
    }

    for (int pageNumber = 0; pageNumber < pageCount; pageNumber++)
    {
        Page page = storageManager.readPage(filename, pageNumber);
        if (page.insertRow(row))
        {
            storageManager.writePage(filename, pageNumber, page);
            return;
        }
    }

    int newPage = storageManager.allocatePage(filename);

    Page page = storageManager.readPage(filename, newPage);

    page.insertRow(row);

    storageManager.writePage(filename, newPage, page);
}

vector<Row> Table::selectAll()
{
    vector<Row> rows;
    int pageCount = storageManager.getPageCount(filename);

    for (int pageNumber = 0; pageNumber < pageCount; pageNumber++)
    {
        Page page = storageManager.readPage(filename, pageNumber);
        for (int i = 0; i < page.getRowCount(); i++)
        {
            Row row;
            page.getRow(i, row);
            rows.push_back(row);
        }
    }
    return rows;
}