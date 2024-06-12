#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

typedef char* (*EncryptFunc)(char*, int);
typedef char* (*DecryptFunc)(char*, int);

int main() {
    void* handle = dlopen("./caesar.dll", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    EncryptFunc encrypt = (EncryptFunc)dlsym(handle, "encrypt");
    if (!encrypt) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    DecryptFunc decrypt = (DecryptFunc)dlsym(handle, "decrypt");
    if (!decrypt) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    char input[100];
    int key;
    printf("Enter the text to encrypt or decrypt: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    printf("Enter the encryption/decryption key: ");
    scanf("%d", &key);
    getchar();

    printf("Choose action:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        printf("Encrypted Text: %s\n", encrypt(input, key));
        break;
    case 2:
        printf("Decrypted Text: %s\n", decrypt(input, key));
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    dlclose(handle);
    return 0;
}
