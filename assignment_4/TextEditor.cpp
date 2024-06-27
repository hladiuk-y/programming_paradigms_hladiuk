#include "TextEditor.h"
#include <iostream>
#include <fstream>
#include <sstream>

TextEditor::TextEditor(const std::string& libPath) : cipher(libPath) {}

void TextEditor::run() {
    char choice;
    do {
        std::cout << "Choose operation:\n"
                  << "1. Encrypt\n"
                  << "2. Decrypt\n"
                  << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case '1': {
                std::string inputPath, outputPath;
                int key;

                std::cout << "Enter input file path: ";
                std::getline(std::cin, inputPath);

                std::cout << "Enter output file path: ";
                std::getline(std::cin, outputPath);

                std::cout << "Enter key for encryption: ";
                std::cin >> key;
                std::cin.ignore();

                performEncrypt(inputPath, outputPath, key);
                break;
            }
            case '2': {
                std::string inputPath, outputPath;
                int key;

                std::cout << "Enter input file path: ";
                std::getline(std::cin, inputPath);

                std::cout << "Enter output file path: ";
                std::getline(std::cin, outputPath);

                std::cout << "Enter key for decryption: ";
                std::cin >> key;
                std::cin.ignore();

                performDecrypt(inputPath, outputPath, key);
                break;
            }
            case '3':
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != '3');
}

void TextEditor::performEncrypt(const std::string& inputPath, const std::string& outputPath, int key) {
    std::ifstream inputFile(inputPath);
    if (!inputFile) {
        std::cerr << "Error: Unable to open input file.\n";
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string content = buffer.str();

    Text text(content);
    Text encryptedText = cipher.encrypt(text, key);

    std::ofstream outputFile(outputPath);
    if (!outputFile) {
        std::cerr << "Error: Unable to open output file.\n";
        return;
    }

    outputFile << encryptedText.getContent();
    std::cout << "Encryption completed. Encrypted text written to " << outputPath << std::endl;
}

void TextEditor::performDecrypt(const std::string& inputPath, const std::string& outputPath, int key) {
    std::ifstream inputFile(inputPath);
    if (!inputFile) {
        std::cerr << "Error: Unable to open input file.\n";
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string content = buffer.str();

    Text text(content);
    Text decryptedText = cipher.decrypt(text, key);

    std::ofstream outputFile(outputPath);
    if (!outputFile) {
        std::cerr << "Error: Unable to open output file.\n";
        return;
    }

    outputFile << decryptedText.getContent();
    std::cout << "Decryption completed. Decrypted text written to " << outputPath << std::endl;
}
