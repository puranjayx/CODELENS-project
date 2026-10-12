
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

enum class TokType
{
    Keyword,
    Identifier,
    Number,
    Operator,
    Symbol
};

struct Token
{
    TokType type;
    string text;
    int line;
};

struct Metrics
{
    unordered_map<string, int> freq;

    int loops = 0;
    int branches = 0;
    int maxLoopDepth = 0;

    bool recursive = false;

    string timeComplexity;
    double score = 0;
};
