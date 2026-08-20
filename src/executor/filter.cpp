#include "filter.h"

Filter::Filter(
    ExecutionOperator& child,
    int targetId)
    : child(child),
      targetId(targetId)
{
}

bool Filter::open()
{
    return child.open();
}

bool Filter::next(Row& row)
{
    while (child.next(row))
    {
        if (row.id == targetId)
        {
            return true;
        }
    }

    return false;
}

void Filter::close()
{
    child.close();
}