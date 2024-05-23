#include <stdio.h> // бібліотека для стандартних вводу/виводу
#include <stdlib.h> // бібліотека для роботи з памʼяттю
#include <string.h> // бібліотека для роботи з рядками

void printHelp();
void appendText(char ***textLines, int *numLines, char *text, int silent);
void startNewLine(char ***textLines, int *numLines);
void printText(char **textLines, int numLines);
void saveTextToFile(char **textLines, int numLines, char *filename);
void loadTextFromFile(char ***textLines, int *numLines, char *filename);
void searchSubstring(char **textLines, int numLines, const char *substring);
void freeMemory(char **textLines, int numLines);
void clearConsole();
//обʼявляємо функціїї на початку програми,
// щоб надалі з будь-якого місця до них був доступ

// * зберігає адресу першого символу рядку
// ** зберігає адресу масива рядків
// *** динамічний масив, зберігає посилання на адресу масиву, дозволяє змінювати не тільки вміст масиву, а і його адресу

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
        } else if (strncmp(input, "search ", 7) == 0) {
            searchSubstring(textLines, numLines, input + 7);
        } else {
            printf("The command '%s' is not implemented.\n", input);
        }
    }

    freeMemory(textLines, numLines);
    return 0;
}

void printHelp() { // іформаційна функція, показуємо юзеру які команди підтримує наша програма
    printf("\nAvailable commands:\n");
    printf("1. help - Print this help information\n");
    printf("2. exit - Exit the program\n");
    printf("3. append <text> - Append text to the stored text\n");
    printf("4. newline - Start a new line\n");
    printf("5. print - Print the stored text\n");
    printf("6. save <filename> - Save the stored text to a file\n");
    printf("7. load <filename> - Load text from a file\n");
    printf("8. search <substring> - Search for a substring in the stored text\n");
    // реалізація просто через звичайну функцію printf
}

void appendText(char ***textLines, int *numLines, char *text, int silent) // функція щоб додавати текст
// передаємо динамічіний масив наших стрінгів, кількість рядків, наш рядок який ми будемо переписувати,
// прапорець, який вказує, чи повинна функція виводити повідомлення
{
    if (*numLines == 0) {
        // якщо ще немає жодного рядка, створюємо новий
        *textLines = malloc(sizeof(char *));
        if (*textLines == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE); // обробка помилки
        }
        *numLines = 1;
        (*textLines)[0] = strdup(""); // створюємо порожній рядок
        if ((*textLines)[0] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE); // обробка помилки
        }
    }

    // отримуємо останній рядок
    int lastLineIndex = *numLines - 1;
    char *currentLine = (*textLines)[lastLineIndex];

    // перевіряємо, чи потрібно додати пробіл перед текстом
    size_t additionalLength = strlen(text) + (strlen(currentLine) > 0 ? 1 : 0);
    size_t newLength = strlen(currentLine) + additionalLength + 1;

    // виділяємо нову пам'ять для доданого тексту
    char *newLine = realloc(currentLine, newLength);
    if (newLine == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // додаємо пробіл, якщо рядок не порожній
    if (strlen(currentLine) > 0) {
        strcat(newLine, " ");
    }
    // додаємо новий текст до кінця поточного рядка
    strcat(newLine, text);
    (*textLines)[lastLineIndex] = newLine;

    if (!silent) {
        printf("Text successfully appended: \"%s\"\n", text);
    }
}

void startNewLine(char ***textLines, int *numLines) // функція щоб додавати новий рядок
// передаємо динамічіний масив наших стрінгів і кількість рядків
{
    (*numLines)++; // додаємо новий рядок
    *textLines = realloc(*textLines, (*numLines) * sizeof(char *)); // виділення памʼяті для зберігання нового рядка
    if (*textLines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE); // обробка помилки
    }
    (*textLines)[(*numLines) - 1] = strdup(""); // шукаємо останній рядок, виділяємо під нього памʼять
    // і записуємо туди порожній рядок
    if ((*textLines)[(*numLines) - 1] == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE); // обробка помилким
    }
    printf("New line is started.\n"); // виводимо повідомлення, що рядок додано
}

void printText(char **textLines, int numLines)
// передаємо наш масив стрінгів і кількість рядків
{
    printf("\nStored text:\n");
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < strlen(textLines[i]); j++) {
            printf("%c", textLines[i][j]); // проходимо по всіх рядках і потім по всіх символах в рядку і виводимо їх
        }
        printf("\n"); // додати новий рядок після кожного текстового рядка
    }
}

void saveTextToFile(char **textLines, int numLines, char *filename) // функція яка записує текст в файл
// передаємо наш масив рядків, кількість рядків і змінну з імʼям файла
{
    FILE *file = fopen(filename, "w"); // відкриваємо файл в режимі запису
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s for writing.\n", filename);
        return; // обробка помилки відкриття файлу
    }
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < strlen(textLines[i]); j++) // проходимо по кожному рядку по кожному символу
        {
            fputc(textLines[i][j], file); // записуємо посимвольно в файл
        }
        fputc('\n', file); // додаємо символ нового рядку
    }
    fclose(file); // закриваємо файл
    printf("Text successfully saved to file: %s\n", filename);
}

void loadTextFromFile(char ***textLines, int *numLines, char *filename)
// передаємо посилання на масив, бо фунція повинна змінити вказівник на масив рядків, виділяючи нову пам'ять і передаючи масив назад
{
    FILE *file = fopen(filename, "r"); // відкриваємо файл в режимі читання
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s for reading.\n", filename);
        return; // обробка помилки відкриття файлу
    }

    freeMemory(*textLines, *numLines); //звільняємо памʼять
    *textLines = NULL; // очищуємо масив
    *numLines = 0;

    char buffer[256]; // створюємо буфер розміром 256 байтів для зчитування рядків з файлу
    while (fgets(buffer, sizeof(buffer), file) != NULL) // читаємо рядки поки не дійдемо до кінця файлу
    {
        appendText(textLines, numLines, buffer, 1); // викликаємо функцію appendText для додавання зчитаного рядка до масиву

    }
    fclose(file); // закриваємо файл
    printf("Text successfully loaded from file: %s\n", filename);
}

void searchSubstring(char **textLines, int numLines, char *substring)
// передаємо масив, кількість рядків, і підрядок, який ми будемо шукати
{
    int subLen = strlen(substring); // рахуємо довжину підрядка
    if (subLen == 0) {
        printf("The substring is empty.\n");
        return; // обробка помилки якщо рядок пустий
    }

    printf("Searching for \"%s\" in the stored text:\n", substring);
    int found = 0; // змінна для знайденого рядку

    for (int i = 0; i < numLines; i++) // проходимось по кожному рядку
    {
        char *line = textLines[i];
        int lineLen = strlen(line); //отримуємо поточний рядок і рахуємо його довжину

        for (int j = 0; j <= lineLen - subLen; j++) // проходимось по кожному елементу рядку, поки він більший або рівний підрядку
        {
            if (strncmp(&line[j], substring, subLen) == 0) {
                printf("Found at line %d, position %d\n", i + 1, j + 1);
                found = 1; // порівнюємо рядки, рахуємо позицію де знайшли збіг і змінюємо значення змінної found,
                // щоб показати, що рядок знайдено
            }
        }
    }

    if (!found) {
        printf("No occurrences of \"%s\" found.\n", substring);
    } // якщо співпадінь нема, то виводимо повідомлення про це
}

void freeMemory(char **textLines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        free(textLines[i]); // звільнення памʼяті де були рядки
    }
    free(textLines); // звільнення памʼяті де був масив
}

void clearConsole() {
    system("clear"); // очищення консолі
}