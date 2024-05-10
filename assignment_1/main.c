#include <stdio.h> // бібліотека для стандартних вводу/виводу
#include <string.h> // бібліотека для роботи з рядками

void printHelp();
void processCommand(const char *command); //обʼявляємо функціїї на початку програми,
// щоб надалі з будь-якого місця до них був доступ

int main() {
    char input[100];

    printf("Welcome to the command parser!\n");

    while (1) {
        printf("\nEnter a command: ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "help") == 0) { // string comparison
            printHelp();
        } else if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        } else {
            processCommand(input);
        }
    }

    return 0;
}

void printHelp() {
    printf("\nAvailable commands:\n");
    printf("1. help - Print this help information\n");
    printf("2. exit - Exit the program\n");
}

void processCommand(const char *command) {
    printf("The command '%s' is not implemented.\n", command);
}
