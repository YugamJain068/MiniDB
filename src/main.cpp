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

        if (query.rfind("CREATE TABLE ", 0) == 0)
        {
            string tableName = query.substr(13);

            if (tableName.back() == ';')
                tableName.pop_back();

            if(db.createTable(tableName))   cout << "Table created\n";
        }

        if (query.rfind("INSERT INTO ", 0) == 0)
        {
            size_t valuesPos = query.find(" VALUES");

            string tableName = query.substr(12, valuesPos - 12);

            size_t start = query.find("(");

            size_t end = query.find(")");

            string values = query.substr(start + 1, end - start - 1);

            stringstream ss(values);

            string idStr;
            getline(ss, idStr, ',');

            int id = stoi(idStr);

            string name;
            getline(ss, name);

            if (name[0] == ' ')
                name.erase(0, 1);

            if (name.front() == '\'')
                name.erase(0, 1);

            if (name.back() == '\'')
                name.pop_back();

            Row row;
            row.id = id;
            strncpy(row.name, name.c_str(), sizeof(row.name) - 1);

            row.name[sizeof(row.name) - 1] = '\0';

            Table table(tableName);
            table.insert(row);

            cout << "1 row inserted" << endl;
        }

        if (query.rfind("SELECT * FROM ", 0) == 0)
        {
            string tableName = query.substr(14);

            if (tableName.back() == ';')
            {
                tableName.pop_back();
            }
            db.selectAll(tableName);
        }

        if (query == "EXIT")
            break;
    }

    return 0;
}