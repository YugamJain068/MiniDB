#ifndef SQL_PARSER_H
#define SQL_PARSER_H
#pragma once

#include "../database/database.h"

#include <string>
#include <memory>
#include <vector>

#include "ast.h"
#include "token.h"

class SQLParser
{
private:
    std::vector<Token> tokens;
    size_t current;

public:
    SQLParser(const std::vector<Token>& tokens);

    std::unique_ptr<Statement> parse();

private:
    const Token& peek() const;
    const Token& advance();
    bool isAtEnd() const;

    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    std::unique_ptr<SelectStatement> parseSelect();
    std::unique_ptr<CreateTableStatement> parseCreate();
    std::unique_ptr<InsertStatement> parseInsert();
};

#endif