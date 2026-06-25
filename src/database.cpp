#include "database.h"
#include "table.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

Database::Database()
{
    ifstream meta("../data/schema.meta");
    string table;

    while (getline(meta, table))
    {
        if (!table.empty())
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
    if (tableExists(tableName))
    {
        cout << "Table already exists\n";
        return false;
    }

    string filename = "../data/" + tableName + ".tbl";
    ofstream tableFile(filename);
    tableFile.close();

    ofstream meta("../data/schema.meta", ios::app);
    meta << tableName << '\n';
    meta.close();

    tables.push_back(tableName);

    return true;
}

void Database::selectAll(const std::string &tableName)
{
    if (!tableExists(tableName))
    {
        cout << "Error: Table '" << tableName << "' does not exist\n";
        return;
    }
    Table table(tableName);

    auto rows = table.selectAll();
    if (rows.empty())
    {
        cout << "No rows found.\n";
        return;
    }

    cout << "+----+--------------------------------+\n";

    cout << "| ID | Name                           |\n";

    cout << "+----+--------------------------------+\n";

    for (const auto &row : rows)
    {
        cout
            << "| "
            << setw(2)
            << left
            << row.id

            << " | "

            << setw(30)
            << left
            << row.name

            << " |\n";
    }

    cout << "+----+--------------------------------+\n";
}