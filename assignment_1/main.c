#include <stdio.h> // бібліотека для стандартних вводу/виводу
#include <stdlib.h> // бібліотека для роботи з памʼяттю
#include <string.h> // бібліотека для роботи з рядками

void printHelp();
void appendText(char ***textLines, int *numLines, const char *text, int silent);
void startNewLine(char ***textLines, int *numLines);
void printText(char **textLines, int numLines);
void saveTextToFile(char **textLines, int numLines, const char *filename);
void loadTextFromFile(char ***textLines, int *numLines, const char *filename);
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

        if (strcmp(input, "help") == 0) {
            printHelp();
        } else if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        } else if (strncmp(input, "append ", 7) == 0) {
            appendText(&textLines, &numLines, input + 7, 0);
        } else if (strcmp(input, "print") == 0) {
            printText(textLines, numLines);
        } else if (strcmp(input, "newline") == 0) {
            startNewLine(&textLines, &numLines);
        } else if (strncmp(input, "save ", 5) == 0) {
            saveTextToFile(textLines, numLines, input + 5);
        } else if (strncmp(input, "load ", 5) == 0) {
            loadTextFromFile(&textLines, &numLines, input + 5);
            printText(textLines, numLines);
        } else {
            printf("The command '%s' is not implemented.\n", input);
        }
    }

    freeMemory(textLines, numLines);
    return 0;
}

void printHelp() {
    printf("\nAvailable commands:\n");
    printf("1. help - Print this help information\n");
    printf("2. exit - Exit the program\n");
    printf("3. append <text> - Append text to the stored text\n");
    printf("4. newline - Start a new line\n");
    printf("5. print - Print the stored text\n");
    printf("6. save <filename> - Save the stored text to a file\n");
    printf("7. load <filename> - Load text from a file\n");
}

void appendText(char ***textLines, int *numLines, const char *text, int silent)
{
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
    if (!silent) {
        printf("Text successfully appended: \"%s\"\n", text);
    }
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

void saveTextToFile(char **textLines, int numLines, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s for writing.\n", filename);
        return;
    }
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < strlen(textLines[i]); j++) {
            fputc(textLines[i][j], file);
        }
        fputc('\n', file);
    }
    fclose(file);
    printf("Text successfully saved to file: %s\n", filename);
}

void loadTextFromFile(char ***textLines, int *numLines, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s for reading.\n", filename);
        return;
    }

    freeMemory(*textLines, *numLines);
    *textLines = NULL;
    *numLines = 0;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        appendText(textLines, numLines, buffer, 1);
    }
    fclose(file);
    printf("Text successfully loaded from file: %s\n", filename);
}

void freeMemory(char **textLines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        free(textLines[i]); // звільнення памʼяті де були рядки
    }
    free(textLines); // звільнення памʼяті де був масив
}

void clearConsole() {
    system("clear");
}