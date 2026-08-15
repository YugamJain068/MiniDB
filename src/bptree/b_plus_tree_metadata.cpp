#include "b_plus_tree_metadata.h"

void initializeMetadata(
    BPlusTreeMetadata& metadata)
{
    metadata.magic =
        BPLUS_INDEX_MAGIC;

    metadata.version =
        BPLUS_INDEX_VERSION;

    metadata.rootPageId = -1;

    metadata.pageCount = 1;
}