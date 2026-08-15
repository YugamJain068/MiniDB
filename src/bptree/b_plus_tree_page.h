#ifndef B_PLUS_TREE_PAGE_H
#define B_PLUS_TREE_PAGE_H

#include <cstdint>
#include "../storage/page.h"

enum class BPlusPageType : uint8_t
{
    INVALID = 0,
    LEAF,
    INTERNAL
};

struct BPlusTreePageHeader
{
    BPlusPageType type;

    uint8_t reserved[3];

    uint16_t size;

    int32_t parentPageId;

    int32_t pageId;
};

void initializeBPlusPage(
    BPlusTreePageHeader& header,
    BPlusPageType type,
    int32_t pageId);

#endif