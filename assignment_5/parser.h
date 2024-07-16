#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <vector>
#include <string>

struct ASTNode {
    virtual ~ASTNode() = default;
    [[nodiscard]] virtual double evaluate() const = 0;
};

struct NumberNode : public ASTNode {
    double value;
    explicit NumberNode(double value);
    [[nodiscard]] double evaluate() const override;
};

struct BinaryOpNode : public ASTNode {
    char op;
    std::shared_ptr<ASTNode> left, right;
    BinaryOpNode(char op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
    [[nodiscard]] double evaluate() const override;
};

struct FunctionNode : public ASTNode {
    std::string func;
    std::shared_ptr<ASTNode> arg1, arg2;
    FunctionNode(std::string  func, std::shared_ptr<ASTNode> arg1, std::shared_ptr<ASTNode> arg2 = nullptr);
    [[nodiscard]] double evaluate() const override;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parse();

private:
    const std::vector<Token>& tokens;
    size_t pos;

    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
};

#endif // PARSER_H
