#include "parser.h"
#include <cmath>
#include <stdexcept>

NumberNode::NumberNode(double value) : value(value) {}

double NumberNode::evaluate() const {
    return value;
}

BinaryOpNode::BinaryOpNode(char op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

double BinaryOpNode::evaluate() const {
    double leftValue = left->evaluate();
    double rightValue = right->evaluate();
    switch (op) {
        case '+': return leftValue + rightValue;
        case '-': return leftValue - rightValue;
        case '*': return leftValue * rightValue;
        case '/': return leftValue / rightValue;
        default: throw std::runtime_error("Unknown operator");
    }
}

FunctionNode::FunctionNode(std::string  func, std::shared_ptr<ASTNode> arg1, std::shared_ptr<ASTNode> arg2)
    : func(std::move(func)), arg1(std::move(arg1)), arg2(std::move(arg2)) {}

double FunctionNode::evaluate() const {
    double value1 = arg1->evaluate();
    if (func == "abs") return std::abs(value1);
    if (func == "pow") return std::pow(value1, arg2->evaluate());
    if (func == "max") return std::max(value1, arg2->evaluate());
    if (func == "min") return std::min(value1, arg2->evaluate());
    throw std::runtime_error("Unknown function");
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

std::shared_ptr<ASTNode> Parser::parse() {
    return parseExpression();
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();
    while (pos < tokens.size() && (tokens[pos].value == "+" || tokens[pos].value == "-")) {
        char op = tokens[pos].value[0];
        pos++;
        auto right = parseTerm();
        left = std::make_shared<BinaryOpNode>(op, left, right);
    }
    return left;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();
    while (pos < tokens.size() && (tokens[pos].value == "*" || tokens[pos].value == "/")) {
        char op = tokens[pos].value[0];
        pos++;
        auto right = parseFactor();
        left = std::make_shared<BinaryOpNode>(op, left, right);
    }
    return left;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
    if (tokens[pos].type == NUMBER) {
        double value = std::stod(tokens[pos].value);
        pos++;
        return std::make_shared<NumberNode>(value);
    }
    if (tokens[pos].type == FUNCTION) {
        std::string func = tokens[pos].value;
        pos++;
        pos++;
        auto arg1 = parseExpression();
        std::shared_ptr<ASTNode> arg2;
        if (tokens[pos].type == COMMA) {
            pos++;
            arg2 = parseExpression();
        }
        pos++;
        return std::make_shared<FunctionNode>(func, arg1, arg2);
    }
    if (tokens[pos].type == LPAREN) {
        pos++;
        auto node = parseExpression();
        pos++;
        return node;
    }
    throw std::runtime_error("Unexpected token");
}
