#include "TextEditor.h"
#include <iostream>

int main() {
    try {
        TextEditor editor("libcaesar.dylib");
        editor.run();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
