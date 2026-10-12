
#include "analyzer.h"


// Check whether a loop is logarithmic
bool isLogLoop(const SyntaxNode& node)
{
    for (int i = 0; i < (int)node.tokens.size(); i++)
    {
        // i *= 2
        if (node.tokens[i].text == "*" &&
            i + 1 < (int)node.tokens.size() &&
            node.tokens[i + 1].text == "=")
        {
            return true;
        }

        // i = i * 2
        if (node.tokens[i].text == "=" &&
            i + 2 < (int)node.tokens.size() &&
            node.tokens[i + 2].text == "*")
        {
            return true;
        }

        // i /= 2
        if (node.tokens[i].text == "/" &&
            i + 1 < (int)node.tokens.size() &&
            node.tokens[i + 1].text == "=")
        {
            return true;
        }
    }

    return false;
}

// Return loop cost
int getLoopCost(const SyntaxNode& node)
{
    if (isLogLoop(node))
        return 1;

    return 2;
}

// Recursively analyze the syntax tree
int analyzeNode(const SyntaxNode& node, int parentCost)
{
    int ownCost = 0;

    if (node.type == NodeType::ForLoop ||
        node.type == NodeType::WhileLoop ||
        node.type == NodeType::DoWhileLoop)
    {
        ownCost = getLoopCost(node);
    }

    int currentCost = parentCost + ownCost;
    int maxCost = currentCost;

    for (const SyntaxNode& child : node.children)
    {
        int childCost = analyzeNode(child, currentCost);

        if (childCost > maxCost)
            maxCost = childCost;
    }

    return maxCost;
}

// Convert cost into a complexity label
string analyzeTimeComplexity(const SyntaxNode& root)
{
    int cost = analyzeNode(root, 0);

    if (cost == 0)
        return "O(1)";

    if (cost == 1)
        return "O(log n)";

    if (cost == 2)
        return "O(n)";

    if (cost == 3)
        return "O(n log n)";

    if (cost == 4)
        return "O(n^2)";

    return "O(n^" + to_string(cost / 2) + ")";
}

// Existing keyword counter
void countKeywords(const vector<Token>& tokens)
{
    int forCount = 0;
    int whileCount = 0;
    int doCount = 0;
    int ifCount = 0;
    int elseCount = 0;
    int switchCount = 0;
    int caseCount = 0;

    for (const Token& t : tokens)
    {
        if (t.type == TokType::Keyword)
        {
            if (t.text == "for")
                forCount++;
            else if (t.text == "while")
                whileCount++;
            else if (t.text == "do")
                doCount++;
            else if (t.text == "if")
                ifCount++;
            else if (t.text == "else")
                elseCount++;
            else if (t.text == "switch")
                switchCount++;
            else if (t.text == "case")
                caseCount++;
        }
    }

    cout << "\nLoop and Conditional Keywords:\n";
    cout << "for     : " << forCount << "\n";
    cout << "while   : " << whileCount << "\n";
    cout << "do      : " << doCount << "\n";
    cout << "if      : " << ifCount << "\n";
    cout << "else    : " << elseCount << "\n";
    cout << "switch  : " << switchCount << "\n";
    cout << "case    : " << caseCount << "\n";
}
