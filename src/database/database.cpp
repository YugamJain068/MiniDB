#include "database.h"
#include "../storage/table.h"

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Database::Database() : bufferPool(2), index(&bufferPool)
{
    ifstream meta("../data/schema.meta");
    if (!meta)
    {
        return;
    }
    string tableName;

    while (getline(meta, tableName))
    {
        if (!tableName.empty())
        {
            tables.emplace(
                tableName,
                Table(tableName, bufferPool));
        }
    }

    meta.close();
}

bool Database::tableExists(const string &tableName)
{
    return tables.find(tableName) != tables.end();
}

bool Database::createTable(const string &tableName)
{
    string filename = "../data/" + tableName + ".tbl";
    ofstream tableFile(filename);
    tableFile.close();

    ofstream meta("../data/schema.meta", ios::app);
    meta << tableName << '\n';
    meta.close();

    tables.emplace(
        tableName,
        Table(tableName, bufferPool));

    return true;
}

vector<Row> Database::selectAll(const std::string &tableName)
{
    return tables.at(tableName).selectAll();
}
Row Database::selectByPointer(const std::string &tableName, const RecordPointer &ptr)
{
    return tables.at(tableName)
        .readRow(
            ptr.pageId,
            ptr.slotId);
}

BufferPoolManager &
Database::getBufferPool()
{
    return bufferPool;
}

BPlusTree &
Database::getIndex()
{
    return index;
}

Table &Database::getTable(const std::string &tableName)
{
    return tables.at(tableName);
}