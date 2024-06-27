#include "CaesarCipher.h"
#include <stdexcept>

CaesarCipher::CaesarCipher(const std::string& libPath) {
    libHandle = dlopen(libPath.c_str(), RTLD_LAZY);
    if (!libHandle) {
        throw std::runtime_error("Unable to load library");
    }

    encryptFunc = (char* (*)(const char*, int))dlsym(libHandle, "encrypt");
    if (!encryptFunc) {
        dlclose(libHandle);
        throw std::runtime_error("Unable to load encrypt function");
    }

    decryptFunc = (char* (*)(const char*, int))dlsym(libHandle, "decrypt");
    if (!decryptFunc) {
        dlclose(libHandle);
        throw std::runtime_error("Unable to load decrypt function");
    }
}

CaesarCipher::~CaesarCipher() {
    if (libHandle) {
        dlclose(libHandle);
    }
}

Text CaesarCipher::encrypt(const Text& rawText, int key) {
    char* result = encryptFunc(rawText.getContent().c_str(), key);
    Text encryptedText(result);
    delete[] result;
    return encryptedText;
}

Text CaesarCipher::decrypt(const Text& encryptedText, int key) {
    char* result = decryptFunc(encryptedText.getContent().c_str(), key);
    Text decryptedText(result);
    delete[] result;
    return decryptedText;
}
