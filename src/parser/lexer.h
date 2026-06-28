#ifndef LEXER_H
#define LEXER_H

#pragma once

#include <string>
#include <vector>

#include "token.h"

class Lexer
{
private:
    std::string input;
    size_t current;

public:
    Lexer(const std::string& input);

    std::vector<Token> tokenize();

private:
    char peek() const;
    char advance();
    bool isAtEnd() const;
};

#endif