#ifndef B_PLUS_TREE_INTERNAL_PAGE_H
#define B_PLUS_TREE_INTERNAL_PAGE_H

#include "b_plus_tree_page.h"

struct InternalEntry
{
    int32_t key;
    int32_t childPageId;
};

constexpr int INTERNAL_MAX_ENTRIES =
    (PAGE_SIZE
     - sizeof(BPlusTreePageHeader)
     - sizeof(int32_t))
    / sizeof(InternalEntry);

struct InternalPage
{
    BPlusTreePageHeader header;

    int32_t firstChildPageId;

    InternalEntry entries[INTERNAL_MAX_ENTRIES];
};

static_assert(
    sizeof(InternalPage) <= PAGE_SIZE,
    "InternalPage exceeds page size");

void initializeInternalPage(
    InternalPage& page,
    int32_t pageId);

#endif