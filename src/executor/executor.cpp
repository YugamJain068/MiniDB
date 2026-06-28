#include "executor.h"

#include "../storage/table.h"
#include "../models/row.h"

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

Executor::Executor(Database &db)
    : database(db)
{
}

void Executor::execute(Statement *stmt)
{
    if (auto create =
            dynamic_cast<CreateTableStatement *>(stmt))
    {
        executeCreate(create);
    }
    else if (auto insert =
                 dynamic_cast<InsertStatement *>(stmt))
    {
        executeInsert(insert);
    }
    else if (auto select =
                 dynamic_cast<SelectStatement *>(stmt))
    {
        executeSelect(select);
    }
    else
    {
        cout << "Unknown statement\n";
    }
}

void Executor::executeCreate(CreateTableStatement *stmt)
{
    if (stmt->tableName.empty())
    {
        cout << "Missing table name\n";
        return;
    }

    if (database.tableExists(stmt->tableName))
    {
        cout << "Table already exists\n";
        return;
    }

    if (database.createTable(stmt->tableName))
    {
        cout << "Table created\n";
    }
}

void Executor::executeInsert(InsertStatement *stmt)
{
    if (!database.tableExists(stmt->tableName))
    {
        cout << "Table not found\n";
        return;
    }

    if (stmt->values.size() != 2)
    {
        cout << "Expected 2 values\n";
        return;
    }

    Row row;

    try
    {
        row.id = stoi(stmt->values[0]);
    }
    catch (...)
    {
        cout << "Invalid ID\n";
        return;
    }

    if (stmt->values[1].length() >= sizeof(row.name))
    {
        cout << "Name too long\n";
        return;
    }

    strncpy(
        row.name,
        stmt->values[1].c_str(),
        sizeof(row.name) - 1);

    row.name[sizeof(row.name) - 1] = '\0';

    Table table(stmt->tableName);

    table.insert(row);

    cout << "1 row inserted\n";
}

void Executor::executeSelect(SelectStatement *stmt)
{
    if (!database.tableExists(stmt->tableName))
    {
        cout << "Table not found\n";
        return;
    }

    auto rows = database.selectAll(stmt->tableName);

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