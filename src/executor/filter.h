#ifndef FILTER_H
#define FILTER_H

#include "execution_operator.h"

class Filter : public ExecutionOperator
{
private:

    ExecutionOperator& child;

    int targetId;

public:

    Filter(
        ExecutionOperator& child,
        int targetId);

    bool open() override;

    bool next(Row& row) override;

    void close() override;
};

#endif