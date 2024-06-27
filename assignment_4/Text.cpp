#include "Text.h"

Text::Text() {
    content = "";
}

Text::Text(const std::string& content) {
    this->content = content;
}

void Text::setContent(const std::string& content) {
    this->content = content;
}

std::string Text::getContent() const {
    return content;
}
