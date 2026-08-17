#include "b_plus_tree_page.h"
#include <cstring>

void initializeBPlusPage(
    BPlusTreePageHeader& header,
    BPlusPageType type,
    int32_t pageId)
{
    header.type = type;

    header.reserved[0] = 0;
    header.reserved[1] = 0;
    header.reserved[2] = 0;

    header.size = 0;

    header.parentPageId = -1;

    header.pageId = pageId;
}

void deserializePageHeader(
    const Page& page,
    BPlusTreePageHeader& header)
{
    std::memcpy(
        &header,
        page.data,
        sizeof(BPlusTreePageHeader));
}