#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

#include "database.h"
#include "table.h"
#include "row.h"

using namespace std;

int main()
{
    Database db;

    string query;

    while (true)
    {
        cout << "MiniDB > ";
        getline(cin, query);

        // EXIT
        if (query == "EXIT")
        {
            break;
        }

        // CREATE TABLE
        else if (query.rfind("CREATE TABLE ", 0) == 0)
        {
            string tableName = query.substr(13);

            if (!tableName.empty() && tableName.back() == ';')
                tableName.pop_back();

            if (tableName.empty())
            {
                cout << "Error: Missing table name\n";
                continue;
            }

            if (db.createTable(tableName))
                cout << "Table created\n";
        }

        // INSERT
        else if (query.rfind("INSERT INTO ", 0) == 0)
        {
            size_t valuesPos = query.find(" VALUES");

            if (valuesPos == string::npos)
            {
                cout << "Error: Invalid INSERT syntax\n";
                continue;
            }

            string tableName = query.substr(12, valuesPos - 12);

            if (!db.tableExists(tableName))
            {
                cout << "Error: Table '" << tableName << "' not found\n";
                continue;
            }

            size_t start = query.find('(');
            size_t end = query.rfind(')');

            if (start == string::npos ||
                end == string::npos ||
                end <= start)
            {
                cout << "Error: Invalid VALUES clause\n";
                continue;
            }

            string values = query.substr(start + 1, end - start - 1);

            stringstream ss(values);

            string idStr;
            string name;

            if (!getline(ss, idStr, ','))
            {
                cout << "Error: Missing ID\n";
                continue;
            }

            if (!getline(ss, name))
            {
                cout << "Error: Missing name\n";
                continue;
            }

            int id;

            try
            {
                id = stoi(idStr);
            }
            catch (...)
            {
                cout << "Error: Invalid ID\n";
                continue;
            }

            if (!name.empty() && name.front() == ' ')
                name.erase(0, 1);

            if (!name.empty() && name.front() == '\'')
                name.erase(0, 1);

            if (!name.empty() && name.back() == '\'')
                name.pop_back();

            Row row;

            if (name.length() >= sizeof(row.name))
            {
                cout << "Error: Name too long\n";
                continue;
            }

            row.id = id;

            strncpy(
                row.name,
                name.c_str(),
                sizeof(row.name) - 1);

            row.name[sizeof(row.name) - 1] = '\0';

            Table table(tableName);

            table.insert(row);

            cout << "1 row inserted\n";
        }

        // SELECT
        else if (query.rfind("SELECT * FROM ", 0) == 0)
        {
            string tableName = query.substr(14);

            if (!tableName.empty() &&
                tableName.back() == ';')
            {
                tableName.pop_back();
            }

            if (tableName.empty())
            {
                cout << "Error: Missing table name\n";
                continue;
            }

            if (!db.tableExists(tableName))
            {
                cout << "Error: Table '" << tableName << "' not found\n";
                continue;
            }

            db.selectAll(tableName);
        }

        // Unknown command
        else
        {
            cout << "Error: Invalid SQL statement\n";
        }
    }

    return 0;
}