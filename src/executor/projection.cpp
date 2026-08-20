#include "projection.h"

#include <cstring>

Projection::Projection(
    ExecutionOperator& child,
    Column column)
    : child(child),
      column(column)
{
}

bool Projection::open()
{
    return child.open();
}

bool Projection::next(Row& row)
{
    Row input{};

    if (!child.next(input))
        return false;

    switch (column)
    {
        case Column::ID:

            row.id = input.id;
            row.name[0] = '\0';

            break;

        case Column::NAME:

            row.id = 0;

            std::strncpy(
                row.name,
                input.name,
                sizeof(row.name) - 1);

            row.name[
                sizeof(row.name) - 1
            ] = '\0';

            break;

        case Column::ALL:

            row = input;

            break;
    }

    return true;
}

void Projection::close()
{
    child.close();
}