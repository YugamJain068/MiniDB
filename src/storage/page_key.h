#ifndef PAGE_KEY_H
#define PAGE_KEY_H

#pragma once

#include <string>

struct PageKey
{
    std::string filename;

    int pageId;

    bool operator==(const PageKey& other) const
    {
        return filename == other.filename &&
               pageId == other.pageId;
    }
    bool operator!=(const PageKey& other) const
{
    return !(*this == other);
}
};

#include <functional>

struct PageKeyHash
{
    std::size_t operator()(const PageKey& key) const
    {
        return std::hash<std::string>()(key.filename)
             ^ (std::hash<int>()(key.pageId) << 1);
    }
};

#endif