#ifndef EXECUTION_OPERATOR_H
#define EXECUTION_OPERATOR_H

#include "../models/row.h"

class ExecutionOperator
{
public:

    virtual ~ExecutionOperator() = default;

    virtual bool open() = 0;

    virtual bool next(Row& row) = 0;

    virtual void close() = 0;
};

#endif