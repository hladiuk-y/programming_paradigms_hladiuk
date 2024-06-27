#ifndef TEXT_H
#define TEXT_H

#include <string>

class Text {
public:
    Text();
    Text(const std::string& content);

    void setContent(const std::string& content);
    std::string getContent() const;

private:
    std::string content;
};

#endif // TEXT_H
