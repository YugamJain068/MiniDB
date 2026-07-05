#include "lru_replacer.h"

#include <iostream>

void LRUReplacer::AccessPage(int pageId)
{
    auto it = pageMap.find(pageId);

    if (it != pageMap.end())
    {
        lruList.erase(it->second);
    }

    lruList.push_front(pageId);

    pageMap[pageId] = lruList.begin();
}

bool LRUReplacer::Victim(int& pageId)
{
    if (lruList.empty())
    {
        return false;
    }

    pageId = lruList.back();

    return true;
}

void LRUReplacer::Remove(int pageId)
{
    auto it = pageMap.find(pageId);

    if (it == pageMap.end())
    {
        return;
    }

    lruList.erase(it->second);

    pageMap.erase(it);
}

void LRUReplacer::Print() const
{
    std::cout << "\nLRU Order\n";

    for (int page : lruList)
    {
        std::cout << page << " ";
    }

    std::cout << '\n';
}