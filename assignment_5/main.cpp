#include <iostream>
#include <string>
#include "tokenizer.h"
#include "parser.h"

int main() {
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input.empty()) continue;

        try {
            auto tokens = tokenize(input);
            Parser parser(tokens);
            auto ast = parser.parse();
            double result = ast->evaluate();
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
