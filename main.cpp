
#include "lexer.h"
#include "analyzer.h"
#include "structure_builder.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <stack>

using namespace std;

// Check whether brackets are balanced
bool checkBrackets(const vector<Token>& tokens)
{
    stack<char> st;

    for (const Token& t : tokens)
    {
        if (t.text == "(" || t.text == "{" || t.text == "[")
        {
            st.push(t.text[0]);
        }
        else if (t.text == ")" || t.text == "}" || t.text == "]")
        {
            if (st.empty())
                return false;

            char top = st.top();
            st.pop();

            if ((t.text == ")" && top != '(') ||
                (t.text == "}" && top != '{') ||
                (t.text == "]" && top != '['))
            {
                return false;
            }
        }
    }

    return st.empty();
}

int main(int argc, char* argv[])
{
    // Check file name
    if (argc < 2)
    {
        cout << "Usage: codelens <file.cpp>\n";
        return 1;
    }

    // Open source file
    ifstream f(argv[1]);

    if (!f)
    {
        cout << "Cannot open file\n";
        return 1;
    }

    // Read complete source code
    stringstream ss;
    ss << f.rdbuf();

    string sourceCode = ss.str();

    // Convert source code into tokens
    vector<Token> tokens = tokenize(sourceCode);

    // Check brackets
    if (checkBrackets(tokens))
        cout << "\nBrackets are balanced\n";
    else
        cout << "\nBrackets are NOT balanced\n";

    // Build syntax tree
    SyntaxNode root = buildSyntaxTree(tokens);

    cout << "\nSyntax Tree:\n";
    printSyntaxTree(root);

    cout << "\nSyntax Tree Information:\n";
    cout << "Tree created successfully\n";
    cout << "Top-level nodes: " << root.children.size() << "\n";

    // NEW: Tree-based time complexity analysis
    cout << "\nTree-Based Time Complexity: "
         << analyzeTimeComplexity(root) << "\n";

    // Count token frequencies
    unordered_map<string, int> freq;

    for (const Token& t : tokens)
        freq[t.text]++;

    // Count tokens by category
    int keywordCount = 0;
    int identifierCount = 0;
    int numberCount = 0;
    int operatorCount = 0;
    int symbolCount = 0;

    for (const Token& t : tokens)
    {
        if (t.type == TokType::Keyword)
            keywordCount++;
        else if (t.type == TokType::Identifier)
            identifierCount++;
        else if (t.type == TokType::Number)
            numberCount++;
        else if (t.type == TokType::Operator)
            operatorCount++;
        else if (t.type == TokType::Symbol)
            symbolCount++;
    }

    // Token type information
    cout << "\nToken Type Information:\n";
    cout << "0 - Keyword    : int, for, if\n";
    cout << "1 - Identifier : i, n, bubbleSort\n";
    cout << "2 - Number     : 0, 1, 10\n";
    cout << "3 - Operator   : =, <, ++, -\n";
    cout << "4 - Symbol     : (, ), {, }, ;\n";

    // Existing keyword information
    countKeywords(tokens);

    // Print category-wise counts
    cout << "\nCategory-wise Token Count:\n";
    cout << "Keywords    : " << keywordCount << "\n";
    cout << "Identifiers : " << identifierCount << "\n";
    cout << "Numbers     : " << numberCount << "\n";
    cout << "Operators   : " << operatorCount << "\n";
    cout << "Symbols     : " << symbolCount << "\n";

    // Print token frequencies
    cout << "\nToken Frequencies:\n";

    for (const auto& item : freq)
        cout << item.first << " : " << item.second << "\n";

    // Print all tokens
    cout << "\nTokens:\n";
    cout << "Line number \t Type \t\t Token\n";

    for (const Token& t : tokens)
    {
        cout << t.line << "\t\t";
        cout << (int)t.type << "\t\t";
        cout << t.text << "\n";
    }

    return 0;
}
