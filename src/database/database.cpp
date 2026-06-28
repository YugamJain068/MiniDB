#include "database.h"
#include "../storage/table.h"

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Database::Database()
{
    ifstream meta("../data/schema.meta");
    if (!meta)
    {
        return;
    }
    string table;

    while (getline(meta, table))
    {
        if (!table.empty() && !tableExists(table))
        {
            tables.push_back(table);
        }
    }

    meta.close();
}

bool Database::tableExists(const string &tableName)
{
    return find(tables.begin(), tables.end(), tableName) != tables.end();
}

bool Database::createTable(const string &tableName)
{
    string filename = "../data/" + tableName + ".tbl";
    ofstream tableFile(filename);
    tableFile.close();

    ofstream meta("../data/schema.meta", ios::app);
    meta << tableName << '\n';
    meta.close();

    tables.push_back(tableName);

    return true;
}

vector<Row> Database::selectAll(const std::string &tableName)
{
    Table table(tableName);
    return table.selectAll();
}