#ifndef B_PLUS_TREE_INTERNAL_PAGE_H
#define B_PLUS_TREE_INTERNAL_PAGE_H

#include "b_plus_tree_page.h"

struct InternalEntry
{
    int32_t key;

    int32_t childPageId;
};

struct InternalPage
{
    BPlusTreePageHeader header;

    int32_t firstChildPageId;

    InternalEntry entries[100];
};

#endif