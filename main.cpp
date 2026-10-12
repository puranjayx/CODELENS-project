
#include "lexer.h"
#include "analyzer.h"
#include "structure_builder.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

// Check whether brackets are balanced
bool checkBrackets(const vector<Token>& tokens)
{
    stack<char> st;

    for (const Token& t : tokens)
    {
        if (t.text == "(" || t.text == "{" || t.text == "[")
            st.push(t.text[0]);

        else if (t.text == ")" || t.text == "}" || t.text == "]")
        {
            if (st.empty())
                return false;

            char top = st.top();
            st.pop();

            if ((t.text == ")" && top != '(') ||
                (t.text == "}" && top != '{') ||
                (t.text == "]" && top != '['))
                return false;
        }
    }

    return st.empty();
}

// Represent supported complexity growth
struct ComplexityInfo
{
    int power;
    int logPower;
};

// Convert complexity label into growth information
bool getComplexityInfo(const string& complexity,
                       ComplexityInfo& info)
{
    info.power = 0;
    info.logPower = 0;

    if (complexity == "O(1)")
        return true;

    if (complexity == "O(log n)")
    {
        info.logPower = 1;
        return true;
    }

    if (complexity == "O(n)")
    {
        info.power = 1;
        return true;
    }

    if (complexity == "O(n log n)")
    {
        info.power = 1;
        info.logPower = 1;
        return true;
    }

    if (complexity == "O(n^2)")
    {
        info.power = 2;
        return true;
    }

    if (complexity == "O(n^3)")
    {
        info.power = 3;
        return true;
    }

    if (complexity == "O(n^4)")
    {
        info.power = 4;
        return true;
    }

    if (complexity == "O(n^5)")
    {
        info.power = 5;
        return true;
    }

    return false;
}

// Read a file and estimate its time complexity
bool getFileComplexity(const string& filename,
                       string& complexity)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Cannot open file: " << filename << "\n";
        return false;
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string sourceCode = buffer.str();
    file.close();

    vector<Token> tokens = tokenize(sourceCode);
    SyntaxNode root = buildSyntaxTree(tokens);

    complexity = analyzeTimeComplexity(root);

    return true;
}

// Compare two programs using asymptotic growth
void compareComplexities(const string& fileA,
                         const string& fileB)
{
    string complexityA, complexityB;

    if (!getFileComplexity(fileA, complexityA))
        return;

    if (!getFileComplexity(fileB, complexityB))
        return;

    ComplexityInfo A, B;

    cout << "\n========== COMPLEXITY COMPARISON ==========\n";

    cout << "Program A: " << fileA << "\n";
    cout << "Estimated Complexity: " << complexityA << "\n\n";

    cout << "Program B: " << fileB << "\n";
    cout << "Estimated Complexity: " << complexityB << "\n\n";

    if (!getComplexityInfo(complexityA, A) ||
        !getComplexityInfo(complexityB, B))
    {
        cout << "Cannot compare: unsupported complexity label.\n";
        return;
    }

    cout << "Limit: T_A(n) / T_B(n), as n -> infinity\n";

    if (A.power == B.power &&
        A.logPower == B.logPower)
    {
        cout << "Limit for representative growth functions = 1\n";
        cout << "Both have the same asymptotic growth order.\n";
    }
    else if (A.power < B.power ||
            (A.power == B.power &&
             A.logPower < B.logPower))
    {
        cout << "Limit = 0\n";
        cout << fileA << " has slower asymptotic growth.\n";
    }
    else
    {
        cout << "Limit = infinity\n";
        cout << fileB << " has slower asymptotic growth.\n";
    }

    cout << "\nNote: Comparison depends on estimated complexity labels.\n";
    cout << "===========================================\n";
}

// Existing single-file analysis and storage
int runSingleFileAnalysis(const string& filename)
{
    ifstream f(filename);

    if (!f)
    {
        cout << "Cannot open file: " << filename << "\n";
        return 1;
    }

    stringstream ss;
    ss << f.rdbuf();

    string sourceCode = ss.str();
    f.close();

    vector<Token> tokens = tokenize(sourceCode);

    if (checkBrackets(tokens))
        cout << "\nBrackets are balanced\n";
    else
        cout << "\nBrackets are NOT balanced\n";

    SyntaxNode root = buildSyntaxTree(tokens);

    cout << "\nSyntax Tree:\n";
    printSyntaxTree(root);

    cout << "\nSyntax Tree Information:\n";
    cout << "Tree created successfully\n";
    cout << "Top-level nodes: " << root.children.size() << "\n";

    string complexity = analyzeTimeComplexity(root);

    cout << "\nTree-Based Time Complexity: "
         << complexity << "\n";

    unordered_map<string, int> freq;

    for (const Token& t : tokens)
        freq[t.text]++;

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

    cout << "\nToken Type Information:\n";
    cout << "0 - Keyword    : int, for, if\n";
    cout << "1 - Identifier : i, n, bubbleSort\n";
    cout << "2 - Number     : 0, 1, 10\n";
    cout << "3 - Operator   : =, <, ++, -\n";
    cout << "4 - Symbol     : (, ), {, }, ;\n";

    countKeywords(tokens);

    cout << "\nCategory-wise Token Count:\n";
    cout << "Keywords    : " << keywordCount << "\n";
    cout << "Identifiers : " << identifierCount << "\n";
    cout << "Numbers     : " << numberCount << "\n";
    cout << "Operators   : " << operatorCount << "\n";
    cout << "Symbols     : " << symbolCount << "\n";

    cout << "\nToken Frequencies:\n";

    for (const auto& item : freq)
        cout << item.first << " : " << item.second << "\n";

    cout << "\nTokens:\n";
    cout << "Line number \t Type \t\t Token\n";

    for (const Token& t : tokens)
    {
        cout << t.line << "\t\t";
        cout << (int)t.type << "\t\t";
        cout << t.text << "\n";
    }

    // ROLE 4: OUTPUT AND STORAGE
    AnalysisRecord record;

    record.programName = filename;
    record.sourceCode = sourceCode;
    record.timeComplexity = complexity;

    record.keywordCount = keywordCount;
    record.identifierCount = identifierCount;
    record.numberCount = numberCount;
    record.operatorCount = operatorCount;
    record.symbolCount = symbolCount;

    for (const Token& t : tokens)
    {
        if (t.text == "for" ||
            t.text == "while" ||
            t.text == "do")
        {
            record.loopCount++;
        }
    }

    ofstream history("history.txt", ios::app);

    if (history.is_open())
    {
        history << "Program: " << record.programName << "\n";
        history << "Time Complexity: "
                << record.timeComplexity << "\n";
        history << "Loop Count: " << record.loopCount << "\n";
        history << "Keywords: " << record.keywordCount << "\n";
        history << "Identifiers: " << record.identifierCount << "\n";
        history << "Numbers: " << record.numberCount << "\n";
        history << "Operators: " << record.operatorCount << "\n";
        history << "Symbols: " << record.symbolCount << "\n";
        history << "Source Code:\n";
        history << record.sourceCode << "\n";
        history << "------------------------------\n";

        history.close();

        cout << "\nAnalysis record saved to history.txt\n";
    }
    else
    {
        cout << "\nError: Could not save analysis history\n";
    }

    return 0;
}

int main(int argc, char* argv[])
{
    // Comparison mode
    if (argc == 4 && string(argv[1]) == "compare")
    {
        compareComplexities(argv[2], argv[3]);
        return 0;
    }

    // Normal analysis mode
    if (argc < 2)
    {
        cout << "Usage:\n";
        cout << "  codelens <file.cpp>\n";
        cout << "  codelens compare <file1.cpp> <file2.cpp>\n";
        return 1;
    }

    return runSingleFileAnalysis(argv[1]);
}
