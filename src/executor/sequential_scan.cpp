#include "sequential_scan.h"

SequentialScan::SequentialScan(
    Table& table,
    BufferPoolManager& bufferPool)
    : table(table),
      bufferPool(bufferPool),
      currentPage(0),
      currentSlot(0),
      pageCount(0),
      opened(false)
{
}

bool SequentialScan::open()
{
    pageCount =
        bufferPool.GetPageCount(
            table.getFilename());

    currentPage = 0;
    currentSlot = 0;

    opened = true;

    return true;
}

bool SequentialScan::next(Row& row)
{
    if (!opened)
        return false;

    while (currentPage < pageCount)
    {
        Page* page =
            bufferPool.FetchPage(
                table.getFilename(),
                currentPage);

        if (page == nullptr)
            return false;

        int rowCount =
            page->getRowCount();

        if (currentSlot < rowCount)
        {
            row =
                page->getRow(currentSlot);

            currentSlot++;

            bufferPool.UnpinPage(
                table.getFilename(),
                currentPage);

            return true;
        }

        bufferPool.UnpinPage(
            table.getFilename(),
            currentPage);

        currentPage++;

        currentSlot = 0;
    }

    return false;
}

void SequentialScan::close()
{
    opened = false;

    currentPage = 0;
    currentSlot = 0;
    pageCount = 0;
}