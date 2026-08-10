#ifndef LRU_REPLACER_H
#define LRU_REPLACER_H

#include <list>
#include <unordered_map>

#include "../storage/page_key.h"

class LRUReplacer
{
private:
    std::list<PageKey> lruList;

    std::unordered_map<
        PageKey,
        std::list<PageKey>::iterator,
        PageKeyHash>
        pageMap;

public:
    void AccessPage(const PageKey &key);

    bool Victim(PageKey &key);

    void Remove(const PageKey &key);
    void Print() const;
};

#endif