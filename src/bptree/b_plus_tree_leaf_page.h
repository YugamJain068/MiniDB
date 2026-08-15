#ifndef B_PLUS_TREE_LEAF_PAGE_H
#define B_PLUS_TREE_LEAF_PAGE_H

#include "b_plus_tree_page.h"
#include "../storage/record_pointer.h"

struct LeafEntry
{
    int32_t key;
    RecordPointer value;
};

constexpr int LEAF_MAX_ENTRIES =
    (PAGE_SIZE
     - sizeof(BPlusTreePageHeader)
     - sizeof(int32_t))
    / sizeof(LeafEntry);

struct LeafPage
{
    BPlusTreePageHeader header;

    int32_t nextPageId;

    LeafEntry entries[LEAF_MAX_ENTRIES];
};

static_assert(
    sizeof(LeafPage) <= PAGE_SIZE,
    "LeafPage exceeds page size");

void initializeLeafPage(
    LeafPage& page,
    int32_t pageId);

#endif