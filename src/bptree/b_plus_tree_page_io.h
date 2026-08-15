#ifndef B_PLUS_TREE_PAGE_IO_H
#define B_PLUS_TREE_PAGE_IO_H

#include "b_plus_tree_leaf_page.h"
#include "b_plus_tree_internal_page.h"
#include "../storage/page.h"

void serializeLeafPage(
    const LeafPage& source,
    Page& destination);

void deserializeLeafPage(
    const Page& source,
    LeafPage& destination);

void serializeInternalPage(
    const InternalPage& source,
    Page& destination);

void deserializeInternalPage(
    const Page& source,
    InternalPage& destination);

#endif