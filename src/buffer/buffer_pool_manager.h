#ifndef BUFFER_POOL_MANAGER_H
#define BUFFER_POOL_MANAGER_H

#include <vector>
#include <unordered_map>

#include "../storage/storage_manager.h"
#include "frame.h"
#include "lru_replacer.h"

class BufferPoolManager
{
private:
    std::vector<Frame> frames;
    StorageManager storageManager;
    std::unordered_map<int, int> pageTable;
    LRUReplacer replacer;

public:
    BufferPoolManager(int poolSize);
    Page *FetchPage(const std::string &filename, int pageId);
    void printFrames() const;
    void printPageTable() const;
    void UnpinPage(int pageId);
    bool IsPagePinned(int pageId);
    void MarkDirty(int pageId);
    void FlushPage(const std::string &filename, int pageId);
    void FlushAllPages(const std::string &filename);
    int FindFreeFrame();
    bool EvictPage(const std::string &filename, int &frameIndex);
    void LoadPageIntoFrame(const std::string &filename, int pageId, int frameIndex);
    void printLRU() const;
};

#endif