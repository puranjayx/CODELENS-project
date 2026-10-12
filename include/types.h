#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class TokType { Keyword, Identifier, Number, Operator, Symbol };

struct Token { TokType type; std::string text; int line; };

struct Metrics {
    std::unordered_map<std::string,int> freq;
    int loops = 0, branches = 0, maxLoopDepth = 0;
    bool recursive = false;
    std::string timeComplexity;
    double score = 0;
};