#include "database.h"
#include "table.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <filesystem>

using namespace std;

bool Database::createTable(const string& tableName)
{
    string filename = "../data/" + tableName + ".tbl";

    if(filesystem::exists(filename))
    {
        cout << "Table already exists\n";
        return false;
    }

    ofstream file(filename);
    file.close();
    tables.push_back(tableName);

    return true;
}

void Database::selectAll(const std::string &tableName)
{
    Table table(tableName);

    auto rows = table.selectAll();

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