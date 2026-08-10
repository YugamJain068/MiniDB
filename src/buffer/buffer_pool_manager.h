#ifndef BUFFER_POOL_MANAGER_H
#define BUFFER_POOL_MANAGER_H

#include <vector>
#include <unordered_map>

#include "../storage/storage_manager.h"
#include "frame.h"
#include "lru_replacer.h"
#include "../storage/page_key.h"

class BufferPoolManager
{
private:
    std::vector<Frame> frames;
    StorageManager storageManager;
    std::unordered_map<
    PageKey,
    int,
    PageKeyHash
> pageTable;
    LRUReplacer replacer;

public:
    BufferPoolManager(int poolSize);
    Page *FetchPage(const std::string &filename, int pageId);
    void printFrames() const;
    void printPageTable() const;
    void UnpinPage(
    const std::string& filename,
    int pageId);
    bool IsPagePinned(const std::string& filename, int pageId);
    void MarkDirty(const std::string& filename, int pageId);
    void FlushPage(const std::string& filename, int pageId);
    void FlushAllPages();
    int FindFreeFrame();
    bool EvictPage(int &frameIndex);
    void LoadPageIntoFrame(const std::string &filename, int pageId, int frameIndex);
    void printLRU() const;
    int GetPageCount(const std::string& filename);
    int AllocatePage(const std::string& filename);
    void FlushFrame(Frame& frame);

    ~BufferPoolManager();
};

#endif