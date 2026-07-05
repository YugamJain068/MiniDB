#ifndef LRU_REPLACER_H
#define LRU_REPLACER_H

#include <list>
#include <unordered_map>

class LRUReplacer
{
private:
    std::list<int> lruList;

    std::unordered_map<int,std::list<int>::iterator> pageMap;

public:

    void AccessPage(int pageId);
    bool Victim(int& pageId);
    void Remove(int pageId);
    void Print() const;
};

#endif