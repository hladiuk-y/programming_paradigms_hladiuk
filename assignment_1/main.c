#include <stdio.h> // бібліотека для стандартних вводу/виводу
#include <stdlib.h> // бібліотека для роботи з памʼяттю
#include <string.h> // бібліотека для роботи з рядками

void printHelp();
void processCommand(const char *command, char ***textLines, int *numLines);
void appendText(char ***textLines, int *numLines, const char *text);
void startNewLine(char ***textLines, int *numLines);
void printText(char **textLines, int numLines);
void freeMemory(char **textLines, int numLines);
void clearConsole();
 //обʼявляємо функціїї на початку програми,
// щоб надалі з будь-якого місця до них був доступ

int main() {
    char **textLines = NULL;
    int numLines = 0;

    printf("Welcome to the command parser!\n");

    while (1) {
        clearConsole();
        printf("\nEnter a command: ");
        char input[100];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "help") == 0) { // string comparison
            printHelp();
        } else if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        } else if (strncmp(input, "append ", 7) == 0) {
            processCommand(input, &textLines, &numLines);
        } else if (strcmp(input, "print") == 0) {
            printText(textLines, numLines);
        } else if (strcmp(input, "newline") == 0) {
            startNewLine(&textLines, &numLines);
        } else {
            printf("Invalid command. Type 'help' for available commands.\n");
        }
    }

    freeMemory(textLines, numLines );
    return 0;
}

void printHelp() {
    printf("\nAvailable commands:\n");
    printf("1. help - Print this help information\n");
    printf("2. exit - Exit the program\n");
    printf("3. append <text> - Append text to the stored text\n");
    printf("4. newline - Start a new line\n");
    printf("5. print - Print the stored text\n");
}

void processCommand(const char *command, char ***textLines, int *numLines) {
    if (strncmp(command, "append ", 7) == 0) {
        appendText(textLines, numLines, command + 7);
    } else {
        printf("The command '%s' is not implemented.\n", command);
    }
}

void appendText(char ***textLines, int *numLines, const char *text) {
    (*numLines)++;
    *textLines = realloc(*textLines, (*numLines) * sizeof(char *)); // виділення памʼяті для зберігання нового рядка
    if (*textLines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE); // обробка помилки
    }
    (*textLines)[(*numLines) - 1] = strdup(text); // копіюємо текст в новий рядок
    if ((*textLines)[(*numLines) - 1] == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE); // обробка помилки
    }
    printf("Text successfully appended: \"%s\"\n", text);
}

void startNewLine(char ***textLines, int *numLines) {
    (*numLines)++; // додаємо новий рядок
    *textLines = realloc(*textLines, (*numLines) * sizeof(char *)); // виділення памʼяті для зберігання нового рядка
    if (*textLines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE); // обробка помилки
    }
    (*textLines)[(*numLines) - 1] = strdup(""); // додаємо порожній рядок
    if ((*textLines)[(*numLines) - 1] == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE); // обробка помилким
    }
    printf("New line is started.\n");
}

void printText(char **textLines, int numLines) {
    printf("\nStored text:\n");
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < strlen(textLines[i]); j++) {
            printf("%c", textLines[i][j]); // проходимо по всіх символах в рядку і виводимо їх
        }
        printf("\n"); // додати новий рядок після кожного текстового рядка
    }
}

void freeMemory(char **textLines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        free(textLines[i]); // звільнення памʼяті де були рядки
    }
    free(textLines); // звільненн памʼяті де був масив
}

void clearConsole() {
    system("clear");
}