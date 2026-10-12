
#include "lexer.h"
#include "analyzer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <stack>

using namespace std;

// Brackets check karne ka function
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
            {
                return false;
            }

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
    // Check karo ki file ka naam diya hai ya nahi
    if (argc < 2)
    {
        cout << "Usage: codelens <file.cpp>\n";
        return 1;
    }

    // File open karo
    ifstream f(argv[1]);

    if (!f)
    {
        cout << "Cannot open file\n";
        return 1;
    }

    // File ka pura content read karo
    stringstream ss;
    ss << f.rdbuf();

    string sourceCode = ss.str();

    // Source code ko tokens mein convert karo
    vector<Token> tokens = tokenize(sourceCode);

    // NEW: Tokens banne ke baad brackets check karo
    if (checkBrackets(tokens))
    {
        cout << "\nBrackets are balanced\n";
    }
    else
    {
        cout << "\nBrackets are NOT balanced\n";
    }

    // Har token ki frequency count karo
    unordered_map<string, int> freq;

    for (const Token& t : tokens)
    {
        freq[t.text]++;
    }

    // Category-wise token count
    int keywordCount = 0;
    int identifierCount = 0;
    int numberCount = 0;
    int operatorCount = 0;
    int symbolCount = 0;

    for (const Token& t : tokens)
    {
        if (t.type == TokType::Keyword)
        {
            keywordCount++;
        }
        else if (t.type == TokType::Identifier)
        {
            identifierCount++;
        }
        else if (t.type == TokType::Number)
        {
            numberCount++;
        }
        else if (t.type == TokType::Operator)
        {
            operatorCount++;
        }
        else if (t.type == TokType::Symbol)
        {
            symbolCount++;
        }
    }

    // Token types ki information
    cout << "\nToken Type Information:\n";

    cout << "0 - Keyword    : int, for, if\n";
    cout << "1 - Identifier : i, n, bubbleSort\n";
    cout << "2 - Number     : 0, 1, 10\n";
    cout << "3 - Operator   : =, <, ++, -\n";
    cout << "4 - Symbol     : (, ), {, ;\n";


    countKeywords(tokens);

    // Category-wise counts print karo
    cout << "\nCategory-wise Token Count:\n";

    cout << "Keywords    : " << keywordCount << "\n";
    cout << "Identifiers : " << identifierCount << "\n";
    cout << "Numbers     : " << numberCount << "\n";
    cout << "Operators   : " << operatorCount << "\n";
    cout << "Symbols     : " << symbolCount << "\n";

    // Har token ki frequency print karo
    cout << "\nToken Frequencies:\n";

    for (const auto& item : freq)
    {
        cout << item.first << " : " << item.second << "\n";
    }

    // Saare tokens print karo
    cout << "\nTokens:\n";

    cout << "line number \t type \t\t token\n";

    for (const Token& t : tokens)
    {
        cout << t.line << "\t\t";
        cout << (int)t.type << "\t\t";
        cout << t.text << "\n";
    }

    return 0;
}
