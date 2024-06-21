#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helper.h"

char* encrypt(const char* rawText, int key) {
    helper_function();
    int length = strlen(rawText);
    char* encryptedText = (char*)malloc((length + 1) * sizeof(char));

    key = key % 26;

    for (int i = 0; i < length; i++) {
        char current = rawText[i];

        if (isupper(current))
            encryptedText[i] = ((current - 'A' + key) % 26) + 'A';
        else if (islower(current))
            encryptedText[i] = ((current - 'a' + key) % 26) + 'a';
        else
            encryptedText[i] = current;
    }

    encryptedText[length] = '\0';
    return encryptedText;
}

char* decrypt(const char* encryptedText, int key) {
    helper_function();
    int length = strlen(encryptedText);
    char* decryptedText = (char*)malloc((length + 1) * sizeof(char));

    key = key % 26;

    for (int i = 0; i < length; i++) {
        char current = encryptedText[i];

        if (isupper(current))
            decryptedText[i] = ((current - 'A' - key + 26) % 26) + 'A';
        else if (islower(current))
            decryptedText[i] = ((current - 'a' - key + 26) % 26) + 'a';
        else
            decryptedText[i] = current;
    }

    decryptedText[length] = '\0';
    return decryptedText;
}