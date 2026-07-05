#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>
#include <string>

#include "../models/row.h"
#include "page.h"

class StorageManager
{
public:

    Page readPage(
        const std::string& filename,
        int pageNumber
    );

    void readPage(const std::string& filename, int pageId, Page& page);

    void writePage(
        const std::string& filename,
        int pageNumber,
        const Page& page
    );

    int allocatePage(
        const std::string& filename
    );
    int getPageCount(const std::string& filename);
};

#endif