#ifndef PROJECTION_H
#define PROJECTION_H

#include "execution_operator.h"

enum class Column
{
    ID,
    NAME,
    ALL
};

class Projection : public ExecutionOperator
{
private:

    ExecutionOperator& child;

    Column column;

public:

    Projection(
        ExecutionOperator& child,
        Column column);

    bool open() override;

    bool next(Row& row) override;

    void close() override;
};

#endif