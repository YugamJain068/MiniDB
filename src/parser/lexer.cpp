#include "lexer.h"

Lexer::Lexer(const std::string &input) : input(input), current(0) {}

bool Lexer::isAtEnd() const
{
    return current >= input.size();
}

char Lexer::peek() const
{
    if (isAtEnd())
        return '\0';

    return input[current];
}

char Lexer::advance()
{
    return input[current++];
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        char c = advance();

        if (std::isspace(c))
        {
            continue;
        }
        else if (c == '*')
        {
            tokens.emplace_back(TokenType::STAR, "*");
        }
        else if (c == ',')
        {
            tokens.emplace_back(TokenType::COMMA, ",");
        }
        else if (c == '(')
        {
            tokens.emplace_back(TokenType::LEFT_PAREN, "(");
        }
        else if (c == ')')
        {
            tokens.emplace_back(TokenType::RIGHT_PAREN, ")");
        }
        else if (c == ';')
        {
            tokens.emplace_back(TokenType::SEMICOLON, ";");
        }
        else if (std::isalpha(c) || c == '_')
        {
            std::string word;
            word += c;

            while (!isAtEnd() &&
                   (std::isalnum(peek()) || peek() == '_'))
            {
                word += advance();
            }

            if (word == "SELECT")
                tokens.emplace_back(TokenType::SELECT, word);
            else if (word == "INSERT")
                tokens.emplace_back(TokenType::INSERT, word);
            else if (word == "CREATE")
                tokens.emplace_back(TokenType::CREATE, word);
            else if (word == "TABLE")
                tokens.emplace_back(TokenType::TABLE, word);
            else if (word == "FROM")
                tokens.emplace_back(TokenType::FROM, word);
            else if (word == "VALUES")
                tokens.emplace_back(TokenType::VALUES, word);
            else if (word == "INTO")
                tokens.emplace_back(TokenType::INTO, word);
            else
                tokens.emplace_back(TokenType::IDENTIFIER, word);
        }
        else if (std::isdigit(c))
        {
            std::string number;
            number += c;

            while (!isAtEnd() && std::isdigit(peek()))
            {
                number += advance();
            }

            tokens.emplace_back(TokenType::NUMBER, number);
        }
        else if (c == '\'')
        {
            std::string str;

            while (!isAtEnd() && peek() != '\'')
            {
                str += advance();
            }

            if (!isAtEnd())
                advance(); // Consume closing quote

            tokens.emplace_back(TokenType::STRING, str);
        }
        else
        {
            tokens.emplace_back(TokenType::UNKNOWN, std::string(1, c));
        }
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "");

    return tokens;
}