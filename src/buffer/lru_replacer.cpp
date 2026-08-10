#include "lru_replacer.h"

#include <iostream>

void LRUReplacer::AccessPage(const PageKey &key)
{
    auto it = pageMap.find(key);

    if (it != pageMap.end())
    {
        lruList.erase(it->second);
    }

    lruList.push_front(key);

    pageMap[key] = lruList.begin();
}

bool LRUReplacer::Victim(PageKey &key)
{
    if (lruList.empty())
    {
        return false;
    }

    key = lruList.back();

    return true;
}

void LRUReplacer::Remove(const PageKey &key)
{
    auto it = pageMap.find(key);

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

    for (const PageKey &page : lruList)
    {
        std::cout
            << "("
            << page.filename
            << ", "
            << page.pageId
            << ") ";
    }

    std::cout << '\n';
}