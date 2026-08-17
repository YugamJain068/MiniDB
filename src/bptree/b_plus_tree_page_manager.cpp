#include "b_plus_tree_page_manager.h"

#include <iostream>

BPlusTreePageManager::BPlusTreePageManager(
    BufferPoolManager& bufferPool,
    const std::string& filename)
    : bufferPool(bufferPool),
      filename(filename)
{
}

int BPlusTreePageManager::allocateLeafPage()
{
    // Actually create a new page on disk
    int pageId =
        bufferPool.AllocatePage(filename);

    if (pageId == -1)
        return -1;

    Page* page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (page == nullptr)
        return -1;

    LeafPage leaf{};

    initializeLeafPage(
        leaf,
        pageId);

    serializeLeafPage(
        leaf,
        *page);

    bufferPool.MarkDirty(
        filename,
        pageId);

    bufferPool.UnpinPage(
        filename,
        pageId);

    return pageId;
}

int BPlusTreePageManager::allocateInternalPage()
{
    int pageId =
        bufferPool.AllocatePage(filename);

    Page* page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (page == nullptr)
        return -1;

    InternalPage internal{};

    initializeInternalPage(
        internal,
        pageId);

    serializeInternalPage(
        internal,
        *page);

    bufferPool.MarkDirty(
        filename,
        pageId);

    bufferPool.UnpinPage(
        filename,
        pageId);

    return pageId;
}

bool BPlusTreePageManager::readLeafPage(
    int pageId,
    LeafPage& leaf)
{
    Page* page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (page == nullptr)
        return false;

    deserializeLeafPage(
        *page,
        leaf);

    bufferPool.UnpinPage(
        filename,
        pageId);

    return true;
}

bool BPlusTreePageManager::writeLeafPage(
    int pageId,
    const LeafPage& leaf)
{
    Page* page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (page == nullptr)
        return false;

    serializeLeafPage(
        leaf,
        *page);

    bufferPool.MarkDirty(
        filename,
        pageId);

    bufferPool.UnpinPage(
        filename,
        pageId);

    return true;
}

bool BPlusTreePageManager::readInternalPage(
    int pageId,
    InternalPage& internal)
{
    Page* page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (page == nullptr)
        return false;

    deserializeInternalPage(
        *page,
        internal);

    bufferPool.UnpinPage(
        filename,
        pageId);

    return true;
}

bool BPlusTreePageManager::writeInternalPage(
    int pageId,
    const InternalPage& internal)
{
    Page* page =
        bufferPool.FetchPage(
            filename,
            pageId);

    if (page == nullptr)
        return false;

    serializeInternalPage(
        internal,
        *page);

    bufferPool.MarkDirty(
        filename,
        pageId);

    bufferPool.UnpinPage(
        filename,
        pageId);

    return true;
}

bool BPlusTreePageManager::initializeIndex()
{
    int pageCount =
        bufferPool.GetPageCount(filename);

    // New index file
    if (pageCount == 0)
    {
        int pageId =
            bufferPool.AllocatePage(filename);

        if (pageId != 0)
        {
            std::cout
                << "ERROR: Metadata page must be page 0\n";

            return false;
        }

        Page* page =
            bufferPool.FetchPage(
                filename,
                0);

        if (page == nullptr)
            return false;

        BPlusTreeMetadata metadata{};

        initializeMetadata(
            metadata);

        serializeMetadata(
            metadata,
            *page);

        bufferPool.MarkDirty(
            filename,
            0);

        bufferPool.UnpinPage(
            filename,
            0);

        return true;
    }

    return true;
}

bool BPlusTreePageManager::readMetadata(
    BPlusTreeMetadata& metadata)
{
    Page* page =
        bufferPool.FetchPage(
            filename,
            0);

    if (page == nullptr)
        return false;

    deserializeMetadata(
        *page,
        metadata);

    bufferPool.UnpinPage(
        filename,
        0);

    if (metadata.magic !=
        BPLUS_INDEX_MAGIC)
    {
        std::cout
            << "Invalid B+ Tree index file\n";

        return false;
    }

    if (metadata.version !=
        BPLUS_INDEX_VERSION)
    {
        std::cout
            << "Unsupported B+ Tree index version\n";

        return false;
    }

    return true;
}

bool BPlusTreePageManager::writeMetadata(
    const BPlusTreeMetadata& metadata)
{
    Page* page =
        bufferPool.FetchPage(
            filename,
            0);

    if (page == nullptr)
        return false;

    serializeMetadata(
        metadata,
        *page);

    bufferPool.MarkDirty(
        filename,
        0);

    bufferPool.UnpinPage(
        filename,
        0);

    return true;
}

const std::string&
BPlusTreePageManager::getFilename() const
{
    return filename;
}
