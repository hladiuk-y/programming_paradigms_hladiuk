#include "tokenizer.h"
#include <sstream>
#include <cctype>

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream stream(input);
    char c;

    while (stream >> c) {
        if (isdigit(c) || c == '.') {
            stream.putback(c);
            double number;
            stream >> number;
            tokens.push_back({NUMBER, std::to_string(number)});
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == ',' || c == '(' || c == ')') {
            TokenType type;
            switch (c) {
            case '+': case '-': case '*': case '/': type = OPERATOR; break;
            case '(': type = LPAREN; break;
            case ')': type = RPAREN; break;
            case ',': type = COMMA; break;
            }
            tokens.push_back({type, std::string(1, c)});
        } else if (isalpha(c)) {
            std::string func(1, c);
            while (stream.peek() != EOF && isalpha(stream.peek())) {
                func += stream.get();
            }
            tokens.push_back({FUNCTION, func});
        }
    }

    return tokens;
}
