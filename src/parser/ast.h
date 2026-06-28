#ifndef AST_H
#define AST_H

#pragma once

#include <string>
#include <vector>

class Statement
{
public:
    virtual ~Statement() = default;
};

class SelectStatement : public Statement
{
public:
    std::string tableName;

    SelectStatement(const std::string &table) : tableName(table) {}
};

class CreateTableStatement : public Statement
{
public:
    std::string tableName;
    CreateTableStatement(const std::string& table) : tableName(table) {}
};

class InsertStatement : public Statement
{
public:
    std::string tableName;
    std::vector<std::string> values;

     InsertStatement( const std::string& table, const std::vector<std::string>& vals)
        : tableName(table), values(vals) {}
};

#endif