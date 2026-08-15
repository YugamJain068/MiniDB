#ifndef B_PLUS_TREE_PAGE_MANAGER_H
#define B_PLUS_TREE_PAGE_MANAGER_H

#include "../buffer/buffer_pool_manager.h"
#include "b_plus_tree_page_io.h"
#include "b_plus_tree_metadata_io.h"

class BPlusTreePageManager
{
private:
    BufferPoolManager &bufferPool;
    std::string filename;

public:
    BPlusTreePageManager(
        BufferPoolManager &bufferPool,
        const std::string &filename);

    int allocateLeafPage();

    int allocateInternalPage();

    bool readLeafPage(
        int pageId,
        LeafPage &leaf);

    bool writeLeafPage(
        int pageId,
        const LeafPage &leaf);

    bool readInternalPage(
        int pageId,
        InternalPage &internal);

    bool writeInternalPage(
        int pageId,
        const InternalPage &internal);

    void markDirty(int pageId);

    void unpinPage(int pageId);

    bool initializeIndex();

    bool readMetadata(
        BPlusTreeMetadata &metadata);

    bool writeMetadata(
        const BPlusTreeMetadata &metadata);

    const std::string& getFilename() const;
};

#endif