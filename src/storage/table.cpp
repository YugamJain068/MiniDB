#include "table.h"

#include <fstream>

using namespace std;

Table::Table(const string &tableName)
{
    filename = "../data/" + tableName + ".tbl";
}

void Table::insert(const Row &row)
{
    storageManager.writeRow(filename, row);
}

vector<Row> Table::selectAll()
{
    return storageManager.readRows(filename);
}