#include "caesar.h"
#include <string>
#include <cctype>
#include <cstring>

char* strdup(const std::string& str) {
    char* cstr = new char[str.size() + 1];
    std::strcpy(cstr, str.c_str());
    return cstr;
}

std::string cpp_encrypt(const std::string& rawText, int key) {
    int length = rawText.length();
    std::string encryptedText(length, '\0');

    key = key % 26;

    for (int i = 0; i < length; i++) {
        char current = rawText[i];

        if (std::isupper(current))
            encryptedText[i] = ((current - 'A' + key) % 26) + 'A';
        else if (std::islower(current))
            encryptedText[i] = ((current - 'a' + key) % 26) + 'a';
        else
            encryptedText[i] = current;
    }

    return encryptedText;
}

std::string cpp_decrypt(const std::string& encryptedText, int key) {
    int length = encryptedText.length();
    std::string decryptedText(length, '\0');

    key = key % 26;

    for (int i = 0; i < length; i++) {
        char current = encryptedText[i];

        if (std::isupper(current))
            decryptedText[i] = ((current - 'A' - key + 26) % 26) + 'A';
        else if (std::islower(current))
            decryptedText[i] = ((current - 'a' - key + 26) % 26) + 'a';
        else
            decryptedText[i] = current;
    }

    return decryptedText;
}

extern "C" {
    char* encrypt(const char* rawText, int key) {
        std::string result = cpp_encrypt(rawText, key);
        return strdup(result);
    }

    char* decrypt(const char* encryptedText, int key) {
        std::string result = cpp_decrypt(encryptedText, key);
        return strdup(result);
    }
}
