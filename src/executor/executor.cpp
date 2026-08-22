#include "executor.h"

#include "sequential_scan.h"
#include "index_scan.h"

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

    Table &table = database.getTable(stmt->tableName);

    RecordPointer ptr =
        table.insert(row);

    std::cout
        << "Stored at Page "
        << ptr.pageId
        << ", Slot "
        << ptr.slotId
        << '\n';
    try
    {
        bool success =
            database
                .getIndex()
                .insert(
                    row.id,
                    ptr);

        if (!success)
        {
            std::cout
                << "Index insertion failed\n";

            return;
        }
    }
    catch (const std::exception &e)
    {
        std::cout
            << e.what()
            << '\n';

        return;
    }
    std::cout
        << "Indexed Key "
        << row.id
        << '\n';
    RecordPointer found =
        database.getIndex().search(row.id);

    if (found.pageId != ptr.pageId ||
        found.slotId != ptr.slotId)
    {
        std::cout
            << "INDEX ERROR!\n";

        std::cout
            << "Expected "
            << ptr.pageId
            << " "
            << ptr.slotId
            << '\n';

        std::cout
            << "Found "
            << found.pageId
            << " "
            << found.slotId
            << '\n';
    }

    std::cout
        << found.pageId
        << " "
        << found.slotId
        << '\n';
    std::cout
        << "1 row inserted\n";
}

void Executor::executeSelect(SelectStatement *stmt)
{
    if (!database.tableExists(stmt->tableName))
    {
        cout << "Table not found\n";
        return;
    }

    Table &table =
        database.getTable(stmt->tableName);

    // ========================================
    // WHERE -> Index Scan
    // ========================================

    if (stmt->hasWhere)
    {
        IndexScan scan(
            table,
            database.getIndex(),
            stmt->whereId);

        scan.open();

        Row row{};

        if (scan.next(row))
        {
            cout
                << row.id
                << " "
                << row.name
                << '\n';
        }
        else
        {
            cout
                << "No rows found.\n";
        }

        scan.close();

        return;
    }

    // ========================================
    // No WHERE -> Sequential Scan
    // ========================================

    cout
        << "Full Table Scan\n";

    SequentialScan scan(
        table,
        database.getBufferPool());

    scan.open();

    Row row{};

    int count = 0;

    cout
        << "+----+--------------------------------+\n";

    cout
        << "| ID | Name                           |\n";

    cout
        << "+----+--------------------------------+\n";

    while (scan.next(row))
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

        count++;
    }

    scan.close();

    cout
        << "+----+--------------------------------+\n";

    if (count == 0)
    {
        cout
            << "No rows found.\n";
    }
}