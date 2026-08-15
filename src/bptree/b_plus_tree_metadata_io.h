#ifndef B_PLUS_TREE_METADATA_IO_H
#define B_PLUS_TREE_METADATA_IO_H

#include "../storage/page.h"
#include "b_plus_tree_metadata.h"

void serializeMetadata(
    const BPlusTreeMetadata& metadata,
    Page& page);

void deserializeMetadata(
    const Page& page,
    BPlusTreeMetadata& metadata);

#endif