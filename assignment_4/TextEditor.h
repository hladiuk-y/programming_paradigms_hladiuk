#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "CaesarCipher.h"
#include <string>

class TextEditor {
public:
    TextEditor(const std::string& libPath);

    void run();

private:
    CaesarCipher cipher;

    void performEncrypt(const std::string& inputPath, const std::string& outputPath, int key);
    void performDecrypt(const std::string& inputPath, const std::string& outputPath, int key);
};

#endif // TEXTEDITOR_H
