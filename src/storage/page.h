#ifndef PAGE_H
#define PAGE_H

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

constexpr std::size_t PAGE_SIZE = 4096;

class Row;

class Page
{
public:

    char data[PAGE_SIZE];

    Page();

    uint16_t getRowCount() const;

    void setRowCount(uint16_t count);

    bool hasSpace(std::size_t rowSize) const;

    bool insertRow(const Row& row);

    bool getRow(int index, Row& row) const;
};

#endif
