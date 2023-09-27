#include "globals.h"

const std::string& Production::getLeftPart() {
    return leftPart;
}

const std::vector<std::string>& Production::getRightPart() {
    return rightPart;
}

void Production::setLeftPart(const std::string& str) {
    leftPart = str;
}

void Production::addRightPart(const std::string& str) {
    rightPart.emplace_back(str);
}

const int& ProductionInCode::getLeftPart() {
    return leftPart;
}

const std::vector<int>& ProductionInCode::getRightPart() {
    return rightPart;
}

void ProductionInCode::setLeftPart(const int symbol) {
    leftPart = symbol;
}

void ProductionInCode::addRightPart(const int symbol) {
    rightPart.emplace_back(symbol);
}
