#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

void getAll();

extern std::unordered_map<int, std::unordered_set<int>> First, Follow;
extern std::vector<std::unordered_set<int>> Predict;
extern std::unordered_map<int, std::unordered_map<int, int>> Table;

#endif // PARSER_H
