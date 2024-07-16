#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>

enum TokenType { NUMBER, OPERATOR, FUNCTION, LPAREN, RPAREN, COMMA };

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);

#endif // TOKENIZER_H
