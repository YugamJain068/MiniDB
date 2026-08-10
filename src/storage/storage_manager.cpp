#include "storage_manager.h"

#include <fstream>

using namespace std;

Page StorageManager::readPage(
    const string &filename,
    int pageNumber)
{
    Page page;

    ifstream file(
        filename,
        ios::binary);

    if (!file)
    {
        return page;
    }

    file.seekg(pageNumber * PAGE_SIZE);

    file.read(
        page.data,
        PAGE_SIZE);

    file.close();

    return page;
}

void StorageManager::writePage(
    const string &filename,
    int pageNumber,
    const Page &page)
{
    fstream file(
        filename,
        ios::binary | ios::in | ios::out);

    if (!file)
    {
        return;
    }

    file.seekp(pageNumber * PAGE_SIZE);

    file.write(
        page.data,
        PAGE_SIZE);

    file.close();
}

int StorageManager::allocatePage(
    const string &filename)
{
    fstream file(
        filename,
        ios::binary | ios::in | ios::out);

    // Create file if it doesn't exist
    if (!file)
    {
        ofstream create(filename, ios::binary);
        create.close();

        file.open(
            filename,
            ios::binary | ios::in | ios::out);
    }

    file.seekg(0, ios::end);

    streampos fileSize = file.tellg();

    int pageNumber = static_cast<int>(
        fileSize / PAGE_SIZE);

    Page emptyPage;

    file.write(
        emptyPage.data,
        PAGE_SIZE);

    file.close();

    return pageNumber;
}

int StorageManager::getPageCount(
    const std::string& filename)
{
    ifstream file(
        filename,
        ios::binary);

    if (!file)
    {
        return 0;
    }

    file.seekg(0, ios::end);

    streampos fileSize = file.tellg();

    file.close();

    return static_cast<int>(
        fileSize / PAGE_SIZE);
}


