#include "token.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::SELECT: return "SELECT";
        case TokenType::INSERT: return "INSERT";
        case TokenType::CREATE: return "CREATE";
        case TokenType::TABLE: return "TABLE";
        case TokenType::INTO: return "INTO";
        case TokenType::VALUES: return "VALUES";
        case TokenType::FROM: return "FROM";

        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";

        case TokenType::STAR: return "STAR";
        case TokenType::COMMA: return "COMMA";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::SEMICOLON: return "SEMICOLON";

        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

Token::Token(TokenType type, const std::string &value): type(type), value(value) {}