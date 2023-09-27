#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <string>

class Production {
    std::string leftPart;
    std::vector<std::string> rightPart;
public:
    const std::string& getLeftPart();
    const std::vector<std::string>& getRightPart();
    void setLeftPart(const std::string& str);
    void addRightPart(const std::string& str);
};

class ProductionInCode {
    int leftPart;
    std::vector<int> rightPart;
public:
    const int& getLeftPart();
    const std::vector<int>& getRightPart();
    void setLeftPart(const int symbol);
    void addRightPart(const int symbol);
};

#endif // GLOBALS_H
