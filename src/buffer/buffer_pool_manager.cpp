#include "buffer_pool_manager.h"

#include <iostream>

BufferPoolManager::BufferPoolManager(int poolSize)
{
    frames.resize(poolSize);
}

void BufferPoolManager::printFrames() const
{
    std::cout << "\nFrames\n\n";

    for (size_t i = 0; i < frames.size(); i++)
    {
        const Frame &frame = frames[i];

        std::cout
            << "Frame " << i
            << " | File " << frame.filename
            << " | Page " << frame.pageId
            << " | Pin " << frame.pinCount
            << " | Dirty " << frame.dirty
            << " | Used " << frame.isUsed
            << '\n';
    }
}

Page *BufferPoolManager::FetchPage(const std::string &filename,
                                   int pageId)
{
    // ----------------------------
    // Step 1 : Check Page Table
    // ----------------------------
    PageKey key{
        filename,
        pageId};
    auto it = pageTable.find(key);

    if (it != pageTable.end())
    {
        Frame &frame = frames[it->second];

        frame.pinCount++;

        std::cout
            << "Pin Count = "
            << frame.pinCount
            << '\n';

        PageKey key{
            filename,
            pageId};

        replacer.AccessPage(key);

        std::cout << "Page found in Buffer Pool\n";

        return &frame.page;
    }

    int frameIndex = FindFreeFrame();

    if (frameIndex == -1)
    {
        if (!EvictPage(frameIndex))
            return nullptr;
    }

    LoadPageIntoFrame(
        filename,
        pageId,
        frameIndex);

    return &frames[frameIndex].page;
}

void BufferPoolManager::FlushFrame(Frame &frame)
{
    if (!frame.dirty)
        return;

    storageManager.writePage(
        frame.filename,
        frame.pageId,
        frame.page);

    frame.dirty = false;
}

void BufferPoolManager::printPageTable() const
{
    std::cout << "\nPage Table\n";

    for (const auto &entry : pageTable)
    {
        std::cout
            << entry.first.filename
            << " Page "
            << entry.first.pageId
            << " -> Frame "
            << entry.second
            << '\n';
    }
}

void BufferPoolManager::UnpinPage(
    const std::string &filename,
    int pageId)
{
    PageKey key{
        filename,
        pageId};

    auto it =
        pageTable.find(key);

    if (it == pageTable.end())
    {
        std::cout << "Page not found\n";
        return;
    }

    Frame &frame = frames[it->second];

    if (frame.pinCount == 0)
    {
        std::cout
            << "Warning: Page already unpinned\n";
        return;
    }

    frame.pinCount--;

    std::cout << "Page "
              << pageId
              << " unpinned. Pin Count = "
              << frame.pinCount
              << '\n';
}

bool BufferPoolManager::IsPagePinned(const std::string &filename, int pageId)
{
    PageKey key{
        filename,
        pageId};
    auto it = pageTable.find(key);

    if (it == pageTable.end())
        return false;

    return frames[it->second].pinCount > 0;
}

void BufferPoolManager::MarkDirty(const std::string &filename, int pageId)
{
    PageKey key{
        filename,
        pageId};

    auto it = pageTable.find(key);
    if (it == pageTable.end())
    {
        std::cout << "Page not found\n";
        return;
    }
    Frame &frame = frames[it->second];
    if (frame.dirty)
        return;
    frame.dirty = true;

    std::cout << "Page " << pageId << " marked dirty\n";
}

void BufferPoolManager::FlushPage(const std::string &filename, int pageId)
{
    PageKey key{
        filename,
        pageId};

    auto it =
        pageTable.find(key);
    if (it == pageTable.end())
    {
        std::cout << "Page not found\n";
        return;
    }

    Frame &frame = frames[it->second];
    FlushFrame(frame);

    std::cout << "Page " << pageId << " flushed to disk\n";
}

void BufferPoolManager::FlushAllPages()
{
    for (Frame &frame : frames)
    {
        if (!frame.isUsed)
            continue;

        FlushFrame(frame);

        std::cout
            << "Flushed "
            << frame.filename
            << " Page "
            << frame.pageId
            << '\n';
    }
}

int BufferPoolManager::FindFreeFrame()
{
    for (size_t i = 0; i < frames.size(); i++)
    {
        if (!frames[i].isUsed)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

bool BufferPoolManager::EvictPage(int &frameIndex)
{
    PageKey victim;

    if (!replacer.Victim(victim))
        return false;

    frameIndex = pageTable[victim];

    Frame &frame = frames[frameIndex];

    if (frame.pinCount > 0)
        return false;

    if (frame.dirty)
    {
        storageManager.writePage(
            frame.filename,
            frame.pageId,
            frame.page);
    }
    std::cout
        << "Evicting "
        << frame.filename
        << " Page "
        << frame.pageId
        << '\n';

    pageTable.erase(victim);

    replacer.Remove(victim);

    return true;
}

void BufferPoolManager::LoadPageIntoFrame(const std::string &filename, int pageId, int frameIndex)
{
    Frame &frame = frames[frameIndex];

    frame.page = storageManager.readPage(filename, pageId);

    frame.filename = filename;
    frame.pageId = pageId;
    frame.pinCount = 1;
    frame.dirty = false;
    frame.isUsed = true;

    PageKey key{
        filename,
        pageId};

    pageTable[key] = frameIndex;
    replacer.AccessPage(key);
}

void BufferPoolManager::printLRU() const
{
    replacer.Print();
}

int BufferPoolManager::GetPageCount(
    const std::string &filename)
{
    return storageManager.getPageCount(filename);
}

int BufferPoolManager::AllocatePage(
    const std::string &filename)
{
    return storageManager.allocatePage(filename);
}

BufferPoolManager::~BufferPoolManager()
{
    FlushAllPages();

    std::cout
        << "Buffer Pool Shutdown Complete\n";
}