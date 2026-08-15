#ifndef B_PLUS_TREE_METADATA_H
#define B_PLUS_TREE_METADATA_H

#include <cstdint>

constexpr uint32_t BPLUS_INDEX_MAGIC = 0x42505431;
constexpr uint32_t BPLUS_INDEX_VERSION = 1;

struct BPlusTreeMetadata
{
    uint32_t magic;

    uint32_t version;

    int32_t rootPageId;

    uint32_t pageCount;

};

static_assert(
    sizeof(BPlusTreeMetadata) <= 4096,
    "Metadata exceeds page size");

void initializeMetadata(
    BPlusTreeMetadata& metadata);

#endif