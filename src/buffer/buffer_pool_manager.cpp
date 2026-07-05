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
            << " | Page " << frame.pageId
            << " | Pin " << frame.pinCount
            << " | Dirty " << frame.dirty
            << '\n';
    }
}

Page *BufferPoolManager::FetchPage(const std::string &filename,
                                   int pageId)
{
    // ----------------------------
    // Step 1 : Check Page Table
    // ----------------------------
    auto it = pageTable.find(pageId);

    if (it != pageTable.end())
    {
        Frame &frame = frames[it->second];

        frame.pinCount++;

        replacer.AccessPage(pageId);

        std::cout << "Page found in Buffer Pool\n";

        return &frame.page;
    }

    int frameIndex = FindFreeFrame();

    if (frameIndex == -1)
    {
        if (!EvictPage(filename, frameIndex))
            return nullptr;
    }

    LoadPageIntoFrame(
        filename,
        pageId,
        frameIndex);

    return &frames[frameIndex].page;
}

void BufferPoolManager::printPageTable() const
{
    std::cout << "\nPage Table\n";

    for (const auto &entry : pageTable)
    {
        std::cout
            << "Page "
            << entry.first
            << " -> Frame "
            << entry.second
            << '\n';
    }
}

void BufferPoolManager::UnpinPage(int pageId)
{
    auto it = pageTable.find(pageId);

    if (it == pageTable.end())
    {
        std::cout << "Page not found\n";
        return;
    }

    Frame &frame = frames[it->second];

    if (frame.pinCount > 0)
    {
        frame.pinCount--;
    }

    std::cout << "Page "
              << pageId
              << " unpinned. Pin Count = "
              << frame.pinCount
              << '\n';
}

bool BufferPoolManager::IsPagePinned(int pageId)
{
    auto it = pageTable.find(pageId);

    if (it == pageTable.end())
        return false;

    return frames[it->second].pinCount > 0;
}

void BufferPoolManager::MarkDirty(int pageId)
{
    auto it = pageTable.find(pageId);
    if (it == pageTable.end())
    {
        std::cout << "Page not found\n";
        return;
    }

    frames[it->second].dirty = true;
    std::cout << "Page " << pageId << " marked dirty\n";
}

void BufferPoolManager::FlushPage(const std::string &filename, int pageId)
{
    auto it = pageTable.find(pageId);
    if (it == pageTable.end())
    {
        std::cout << "Page not found\n";
        return;
    }

    Frame &frame = frames[it->second];
    if (!frame.dirty)
    {
        std::cout << "Page " << pageId << " is clean\n";
        return;
    }

    storageManager.writePage(filename, pageId, frame.page);
    frame.dirty = false;

    std::cout << "Page " << pageId << " flushed to disk\n";
}

void BufferPoolManager::FlushAllPages(const std::string &filename)
{
    for (Frame &frame : frames)
    {
        if (!frame.isUsed)
            continue;

        if (!frame.dirty)
            continue;

        storageManager.writePage(filename, frame.pageId, frame.page);
        frame.dirty = false;

        std::cout << "Flushed Page " << frame.pageId << '\n';
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

bool BufferPoolManager::EvictPage(const std::string &filename, int &frameIndex)
{
    int victimPage;

    if (!replacer.Victim(victimPage))
        return false;
    
    

    frameIndex = pageTable[victimPage];

    Frame &frame = frames[frameIndex];

    if (frame.pinCount > 0)
        return false;

    if (frame.dirty)
    {
        storageManager.writePage(
            filename,
            victimPage,
            frame.page);
    }
    std::cout<<"Evicting Page "<<victimPage<<std::endl;

    pageTable.erase(victimPage);

    replacer.Remove(victimPage);

    return true;
}

void BufferPoolManager::LoadPageIntoFrame(const std::string &filename, int pageId, int frameIndex)
{
    Frame &frame = frames[frameIndex];

    frame.page = storageManager.readPage(filename, pageId);

    frame.pageId = pageId;
    frame.pinCount = 1;
    frame.dirty = false;
    frame.isUsed = true;

    pageTable[pageId] = frameIndex;

    replacer.AccessPage(pageId);
}

void BufferPoolManager::printLRU() const
{
    replacer.Print();
}