#ifndef B_PLUS_TREE_LEAF_PAGE_H
#define B_PLUS_TREE_LEAF_PAGE_H

#include "b_plus_tree_page.h"
#include "../storage/record_pointer.h"

struct LeafEntry
{
    int32_t key;

    RecordPointer value;
};

struct LeafPage
{
    BPlusTreePageHeader header;

    int32_t nextPageId;

    LeafEntry entries[100];
};

#endif