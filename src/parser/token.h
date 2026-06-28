#ifndef TOKEN_H
#define TOKEN_H

#pragma once

#include <string>

enum class TokenType
{
    // Keywords
    SELECT,
    INSERT,
    CREATE,
    TABLE,
    INTO,
    VALUES,
    FROM,

    // User-defined names
    IDENTIFIER,

    // Literals
    NUMBER,
    STRING,

    // Symbols
    STAR,
    COMMA,
    LEFT_PAREN,
    RIGHT_PAREN,
    SEMICOLON,

    // End of input
    END_OF_FILE,

    // Invalid token
    UNKNOWN
};

class Token
{
public:
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string &value);
};

std::string tokenTypeToString(TokenType type);

#endif