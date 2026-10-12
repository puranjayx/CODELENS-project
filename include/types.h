
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Token types
enum class TokType
{
    Keyword,
    Identifier,
    Number,
    Operator,
    Symbol
};

// Stores one token
struct Token
{
    TokType type;
    string text;
    int line;
};

// Syntax tree node types
enum class NodeType
{
    Program,
    Function,
    ForLoop,
    WhileLoop,
    DoWhileLoop,
    IfStatement,
    ElseStatement,
    SwitchStatement,
    Block,
    Statement
};

// One syntax tree node
struct SyntaxNode
{
    NodeType type = NodeType::Statement;

    // Original tokens belonging to this node
    vector<Token> tokens;

    // Nested structures inside this node
    vector<SyntaxNode> children;

    int line = 0;
};

// Complexity analysis results
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
