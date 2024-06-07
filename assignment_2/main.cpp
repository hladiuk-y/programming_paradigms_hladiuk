#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stack>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256

class Text {
private:
    char* lines[MAX_LINES]{};
    int numLines;

    struct Command {
        char* lines[MAX_LINES]{};
        int numLines{};
    };

    std::stack<Command> undoStack;
    std::stack<Command> redoStack;

    void saveState() {
        Command state;
        state.numLines = numLines;
        for (int i = 0; i < numLines; ++i) {
            state.lines[i] = strdup(lines[i]);
        }
        undoStack.push(state);
    }

    static void clearCommandLines(Command& command) {
        for (int i = 0; i < command.numLines; ++i) {
            free(command.lines[i]);
        }
    }

public:
    Text() : numLines(0) {
        for (auto & line : lines) {
            line = nullptr;
        }
    }

    ~Text() {
        clear();
    }

    void append(const char* text, int silent = 0) {
        saveState();
        if (numLines == 0) {
            lines[numLines] = strdup("");
            if (lines[numLines] == nullptr) {
                std::cerr << "Memory allocation failed." << std::endl;
                exit(EXIT_FAILURE);
            }
            numLines++;
        }

        int lastLineIndex = numLines - 1;
        char* currentLine = lines[lastLineIndex];
        size_t additionalLength = strlen(text);
        size_t newLength = strlen(currentLine) + additionalLength + 1;

        char* newLine = static_cast<char*>(realloc(currentLine, newLength));
        if (newLine == nullptr) {
            std::cerr << "Memory allocation failed." << std::endl;
            exit(EXIT_FAILURE);
        }

        strcat(newLine, text);
        lines[lastLineIndex] = newLine;

        if (!silent) {
            std::cout << "Text successfully appended: \"" << text << "\"" << std::endl;
        }
    }

    void insert(int lineIndex, int charIndex, const char* text, bool replace = false) {
        saveState();
        if (lineIndex < 0 || lineIndex >= numLines) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        char* currentLine = lines[lineIndex];
        size_t currentLength = strlen(currentLine);
        if (charIndex < 0 || charIndex > currentLength) {
            std::cerr << "Invalid character index." << std::endl;
            return;
        }

        size_t textLength = strlen(text);
        size_t newLength = replace ? charIndex + textLength + currentLength - charIndex : currentLength + textLength + 1;
        char* newLine = static_cast<char*>(malloc(newLength));
        if (newLine == nullptr) {
            std::cerr << "Memory allocation failed." << std::endl;
            exit(EXIT_FAILURE);
        }

        memcpy(newLine, currentLine, charIndex);
        memcpy(newLine + charIndex, text, textLength);
        if (replace) {
            memcpy(newLine + charIndex + textLength, currentLine + charIndex + textLength, currentLength - charIndex - textLength + 1);
        } else {
            memcpy(newLine + charIndex + textLength, currentLine + charIndex, currentLength - charIndex + 1);
        }

        free(currentLine);
        lines[lineIndex] = newLine;

        std::cout << "Text successfully " << (replace ? "replaced" : "inserted") << ": \"" << text << "\"" << std::endl;
    }

    void deleteText(int lineIndex, int startIndex, int endIndex) {
        saveState();
        if (lineIndex < 0 || lineIndex >= numLines) {
            std::cerr << "Invalid line index." << std::endl;
            return;
        }

        char* currentLine = lines[lineIndex];
        size_t currentLength = strlen(currentLine);
        if (startIndex < 0 || endIndex > currentLength || startIndex > endIndex) {
            std::cerr << "Invalid start or end index." << std::endl;
            return;
        }

        size_t newLength = currentLength - (endIndex - startIndex + 1) + 1;
        char* newLine = static_cast<char*>(malloc(newLength));
        if (newLine == nullptr) {
            std::cerr << "Memory allocation failed." << std::endl;
            exit(EXIT_FAILURE);
        }

        memcpy(newLine, currentLine, startIndex);
        memcpy(newLine + startIndex, currentLine + endIndex + 1, currentLength - endIndex);

        free(currentLine);
        lines[lineIndex] = newLine;

        std::cout << "Deleted successfully!" << std::endl;
    }

    void undo() {
        if (undoStack.empty()) {
            std::cout << "No actions to undo." << std::endl;
            return;
        }

        Command currentState;
        currentState.numLines = numLines;
        for (int i = 0; i < numLines; ++i) {
            currentState.lines[i] = strdup(lines[i]);
        }
        redoStack.push(currentState);

        Command lastState = undoStack.top();
        undoStack.pop();
        clear();
        numLines = lastState.numLines;
        for (int i = 0; i < numLines; ++i) {
            lines[i] = strdup(lastState.lines[i]);
        }
        clearCommandLines(lastState);
        std::cout << "Undo successful." << std::endl;
    }

    void redo() {
        if (redoStack.empty()) {
            std::cout << "No actions to redo." << std::endl;
            return;
        }

        Command currentState;
        currentState.numLines = numLines;
        for (int i = 0; i < numLines; ++i) {
            currentState.lines[i] = strdup(lines[i]);
        }
        undoStack.push(currentState);

        Command nextState = redoStack.top();
        redoStack.pop();
        clear();
        numLines = nextState.numLines;
        for (int i = 0; i < numLines; ++i) {
            lines[i] = strdup(nextState.lines[i]);
        }
        clearCommandLines(nextState);
        std::cout << "Redo successful." << std::endl;
    }

    void startNewLine() {
        saveState();
        lines[numLines] = strdup("");
        if (lines[numLines] == nullptr) {
            std::cerr << "Memory allocation failed." << std::endl;
            exit(EXIT_FAILURE);
        }
        numLines++;
        std::cout << "New line is started." << std::endl;
    }

    void appendNewLine(const char* text) {
        saveState();
        lines[numLines] = strdup(text);
        if (lines[numLines] == nullptr) {
            std::cerr << "Memory allocation failed." << std::endl;
            exit(EXIT_FAILURE);
        }
        numLines++;
    }

    void print() const {
        std::cout << "\nStored text:\n";
        for (int i = 0; i < numLines; ++i) {
            std::cout << lines[i] << std::endl;
        }
    }

    void search(const char* substring) const {
        std::cout << "Searching for \"" << substring << "\" in the stored text:\n";
        bool found = false;
        for (int i = 0; i < numLines; ++i) {
            const char* line = lines[i];
            const char* pos = strstr(line, substring);
            while (pos) {
                std::cout << "Found at line " << i << ", position " << (pos - line) << '\n';
                found = true;
                pos = strstr(pos + 1, substring);
            }
        }
        if (!found) {
            std::cout << "No occurrences of \"" << substring << "\" found.\n";
        }
    }

    [[nodiscard]] const char* const* getLines() const {
        return lines;
    }

    [[nodiscard]] int getNumLines() const {
        return numLines;
    }

    void clear() {
        for (int i = 0; i < numLines; ++i) {
            free(lines[i]);
            lines[i] = nullptr;
        }
        numLines = 0;
    }
};

class FileHandler {
public:
    static void saveTextToFile(const Text& text, const char* filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Could not open file " << filename << " for writing." << std::endl;
            return;
        }
        for (int i = 0; i < text.getNumLines(); ++i) {
            file << text.getLines()[i] << '\n';
        }
        std::cout << "Text successfully saved to file: " << filename << std::endl;
    }

    static void loadTextFromFile(Text& text, const char* filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Could not open file " << filename << " for reading." << std::endl;
            return;
        }
        text.clear();
        char buffer[MAX_LINE_LENGTH];
        while (file.getline(buffer, MAX_LINE_LENGTH)) {
            text.appendNewLine(buffer);
        }
        std::cout << "Text successfully loaded from file: " << filename << std::endl;
    }
};

class CommandParser {
private:
    Text& text;
    FileHandler fileHandler = FileHandler();

    // static void clearConsole() {
    //     system("clear");
    // }

public:
    explicit CommandParser(Text& txt) : text(txt) {}

    void parse(const char* input) const {
        char cmd[20];
        sscanf(input, "%s", cmd);
        // clearConsole();
        if (strcmp(cmd, "help") == 0) {
            printHelp();
        } else if (strcmp(cmd, "exit") == 0) {
            std::cout << "Exiting...\n";
        } else if (strncmp(cmd, "append", 6) == 0) {
            const char* appendedText = input + 7;
            text.append(appendedText);
        } else if (strcmp(cmd, "newline") == 0) {
            text.startNewLine();
        } else if (strcmp(cmd, "print") == 0) {
            text.print();
        } else if (strncmp(cmd, "save", 4) == 0) {
            char filename[100];
            sscanf(input + 5, "%s", filename);
            FileHandler::saveTextToFile(text, filename);
        } else if (strncmp(cmd, "load", 4) == 0) {
            char filename[100];
            sscanf(input + 5, "%s", filename);
            FileHandler::loadTextFromFile(text, filename);
            text.print();
        } else if (strncmp(cmd, "search", 6) == 0) {
            const char* substring = input + 7;
            text.search(substring);
        } else if (strncmp(cmd, "undo", 4) == 0) {
            text.undo();
        } else if (strncmp(cmd, "redo", 4) == 0) {
            text.redo();
        } else if (strncmp(cmd, "insert", 6) == 0) {
            int lineIndex, charIndex;
            char word[MAX_LINE_LENGTH];
            sscanf(input + 7, "%d %d %s", &lineIndex, &charIndex, word);
            text.insert(lineIndex, charIndex, word);
        } else if (strncmp(cmd, "replace", 7) == 0) {
            int lineIndex, charIndex;
            char word[MAX_LINE_LENGTH];
            sscanf(input + 8, "%d %d %s", &lineIndex, &charIndex, word);
            text.insert(lineIndex, charIndex, word, true);
        } else if (strncmp(cmd, "delete", 6) == 0) {
            int lineIndex, startIndex, endIndex;
            sscanf(input + 7, "%d %d %d", &lineIndex, &startIndex, &endIndex);
            text.deleteText(lineIndex, startIndex, endIndex);
        } else {
            std::cout << "The command '" << cmd << "' is not implemented.\n";
        }
    }

    static void printHelp() {
        std::cout << "Available commands:\n"
                  << "help - Show this help message\n"
                  << "exit - Exit the program\n"
                  << "append <text> - Append text to the end of the document\n"
                  << "newline - Start a new line\n"
                  << "print - Print the document\n"
                  << "save <filename> - Save the document to a file\n"
                  << "load <filename> - Load the document from a file\n"
                  << "search <substring> - Search for a substring in the document\n"
                  << "undo - Undo the last action\n"
                  << "redo - Redo the last undone action\n"
                  << "insert <line> <index> <text> - Insert text at a specific line and index\n"
                  << "replace <line> <index> <text> - Replace text at a specific line and index\n"
                  << "delete <line> <start> <end> - Delete text from start to end index on a specific line\n";
    }
};

int main() {
    Text text;
    CommandParser parser(text);

    std::cout << "Welcome to the command parser!\n";

    while (true) {
        std::cout << "\nEnter a command: ";
        char input[MAX_LINE_LENGTH];
        std::cin.getline(input, MAX_LINE_LENGTH);
        parser.parse(input);
        if (strcmp(input, "exit") == 0) {
            break;
        }
    }

    return 0;
}
