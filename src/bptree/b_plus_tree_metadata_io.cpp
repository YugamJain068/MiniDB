#include "b_plus_tree_metadata_io.h"

#include <cstring>

void serializeMetadata(
    const BPlusTreeMetadata& metadata,
    Page& page)
{
    std::memset(
        page.data,
        0,
        PAGE_SIZE);

    std::memcpy(
        page.data,
        &metadata,
        sizeof(BPlusTreeMetadata));
}

void deserializeMetadata(
    const Page& page,
    BPlusTreeMetadata& metadata)
{
    std::memcpy(
        &metadata,
        page.data,
        sizeof(BPlusTreeMetadata));
}