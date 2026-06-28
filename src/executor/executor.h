#ifndef EXECUTOR_H
#define EXECUTOR_H

#pragma once

#include "../database/database.h"
#include "../parser/ast.h"

class Executor
{
private:
    Database &database;

public:
    Executor(Database &db);

    void execute(Statement *stmt);

private:
    void executeCreate(CreateTableStatement *stmt);

    void executeInsert(InsertStatement *stmt);

    void executeSelect(SelectStatement *stmt);
};

#endif