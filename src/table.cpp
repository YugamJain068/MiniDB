#include "table.h"

#include <fstream>

using namespace std;

Table::Table(const string &tableName)
{
    filename =
        "../data/" +
        tableName +
        ".tbl";
}

void Table::insert(const Row &row)
{
    ofstream file(
        filename,
        ios::binary | ios::app);

    file.write(
        reinterpret_cast<const char *>(&row),
        sizeof(Row));
}

vector<Row> Table::selectAll()
{
    vector<Row> rows;

    ifstream file(
        filename,
        ios::binary);

    Row row;

    while (
        file.read(
            reinterpret_cast<char *>(&row),
            sizeof(Row)))
    {
        rows.push_back(row);
    }

    return rows;
}