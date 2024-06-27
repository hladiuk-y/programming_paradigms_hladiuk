#ifndef CAESAR_H
#define CAESAR_H

#ifdef __cplusplus
extern "C" {
#endif

    char* encrypt(const char* rawText, int key);
    char* decrypt(const char* encryptedText, int key);

#ifdef __cplusplus
}
#endif

#endif // CAESAR_H
