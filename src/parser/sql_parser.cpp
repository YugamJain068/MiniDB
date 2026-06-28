#include "sql_parser.h"
#include "../storage/table.h"
#include "../models/row.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>

using namespace std;

SQLParser::SQLParser(const std::vector<Token>& tokens)
    : tokens(tokens), current(0)
{
}

const Token& SQLParser::peek() const
{
    return tokens[current];
}

const Token& SQLParser::advance()
{
    if (!isAtEnd())
        current++;

    return tokens[current - 1];
}

bool SQLParser::isAtEnd() const
{
    return peek().type == TokenType::END_OF_FILE;
}

bool SQLParser::match(TokenType type)
{
    if (peek().type == type)
    {
        advance();
        return true;
    }

    return false;
}

Token SQLParser::consume(TokenType type,
                         const std::string& message)
{
    if (peek().type == type)
        return advance();

    throw std::runtime_error(message);
}

std::unique_ptr<Statement> SQLParser::parse()
{
    if (match(TokenType::SELECT))
        return parseSelect();

    if (match(TokenType::CREATE))
        return parseCreate();

    if (match(TokenType::INSERT))
        return parseInsert();

    throw std::runtime_error("Unknown SQL statement");
}

std::unique_ptr<SelectStatement> SQLParser::parseSelect()
{
    consume(TokenType::STAR,
            "Expected *");

    consume(TokenType::FROM,
            "Expected FROM");

    Token table =
        consume(TokenType::IDENTIFIER,
                "Expected table name");

    consume(TokenType::SEMICOLON,
            "Expected ;");

    return std::make_unique<SelectStatement>(
        table.value
    );
}


std::unique_ptr<CreateTableStatement>
SQLParser::parseCreate()
{
    consume(TokenType::TABLE,
            "Expected TABLE");

    Token table =
        consume(TokenType::IDENTIFIER,
                "Expected table name");

    consume(TokenType::SEMICOLON,
            "Expected ;");

    return std::make_unique<CreateTableStatement>(
        table.value
    );
}

std::unique_ptr<InsertStatement>
SQLParser::parseInsert()
{
    consume(TokenType::INTO,
            "Expected INTO");

    Token table =
        consume(TokenType::IDENTIFIER,
                "Expected table name");

    consume(TokenType::VALUES,
            "Expected VALUES");

    consume(TokenType::LEFT_PAREN,
            "Expected (");

    std::vector<std::string> values;

    values.push_back(
        advance().value
    );

    consume(TokenType::COMMA,
            "Expected ,");

    values.push_back(
        advance().value
    );

    consume(TokenType::RIGHT_PAREN,
            "Expected )");

    consume(TokenType::SEMICOLON,
            "Expected ;");

    return std::make_unique<InsertStatement>(
        table.value,
        values
    );
}