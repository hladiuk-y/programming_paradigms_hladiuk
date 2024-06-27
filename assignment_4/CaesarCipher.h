#ifndef CAESARCIPHER_H
#define CAESARCIPHER_H

#include <string>
#include <dlfcn.h>
#include "Text.h"

class CaesarCipher {
public:
    CaesarCipher(const std::string& libPath);
    ~CaesarCipher();

    Text encrypt(const Text& rawText, int key);
    Text decrypt(const Text& encryptedText, int key);

private:
    void* libHandle;
    char* (*encryptFunc)(const char*, int);
    char* (*decryptFunc)(const char*, int);
};

#endif // CAESARCIPHER_H
