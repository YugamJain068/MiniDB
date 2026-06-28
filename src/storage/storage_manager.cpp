#include "storage_manager.h"

#include <fstream>

using namespace std;

void StorageManager::writeRow(
    const string& filename,
    const Row& row)
{
    ofstream file(
        filename,
        ios::binary | ios::app
    );

    if (!file)
    {
        return;
    }

    file.write(
        reinterpret_cast<const char*>(&row),
        sizeof(Row)
    );

    file.close();
}

vector<Row> StorageManager::readRows(
    const string& filename)
{
    vector<Row> rows;

    ifstream file(
        filename,
        ios::binary
    );

    if (!file)
    {
        return rows;
    }

    Row row;

    while (
        file.read(
            reinterpret_cast<char*>(&row),
            sizeof(Row)
        )
    )
    {
        rows.push_back(row);
    }

    file.close();

    return rows;
}