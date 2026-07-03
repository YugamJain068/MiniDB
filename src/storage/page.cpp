#include "page.h"
#include <cstring>

#include "../models/row.h"

Page::Page()
{
    std::memset(data, 0, PAGE_SIZE);

    setRowCount(0);
}

uint16_t Page::getRowCount() const
{
    uint16_t count;

    std::memcpy(
        &count,
        data,
        sizeof(uint16_t));

    return count;
}

void Page::setRowCount(uint16_t count)
{
    std::memcpy(
        data,
        &count,
        sizeof(uint16_t));
}

bool Page::hasSpace(std::size_t rowSize) const
{
    std::size_t usedSpace =
        sizeof(uint16_t) +
        getRowCount() * rowSize;

    return usedSpace + rowSize <= PAGE_SIZE;
}

bool Page::insertRow(const Row& row)
{
    if (!hasSpace(sizeof(Row)))
    {
        return false;
    }

    uint16_t rowCount = getRowCount();

    std::size_t offset =
        sizeof(uint16_t) +
        rowCount * sizeof(Row);

    std::memcpy(
        data + offset,
        &row,
        sizeof(Row));

    setRowCount(rowCount + 1);

    return true;
}

bool Page::getRow(
    int index,
    Row& row) const
{
    if (index < 0 ||
        index >= getRowCount())
    {
        return false;
    }

    std::size_t offset =
        sizeof(uint16_t) +
        index * sizeof(Row);

    std::memcpy(
        &row,
        data + offset,
        sizeof(Row));

    return true;
}