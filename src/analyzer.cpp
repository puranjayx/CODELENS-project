
#include "analyzer.h"
#include <stack>

void countKeywords(const vector<Token>& tokens)
{
    int forCount = 0;
    int whileCount = 0;
    int doCount = 0;
    int ifCount = 0;
    int elseCount = 0;
    int switchCount = 0;
    int caseCount = 0;

    // Loop nesting ke liye
    int loopDepth = 0;
    int maxDepth = 0;

    stack<bool> st;
    vector<bool> loopBody(tokens.size(), false);

    // Identify loop bodies that use braces
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].text == "for" ||
            tokens[i].text == "while" ||
            tokens[i].text == "do")
        {
            int j = i + 1;

            if (tokens[i].text == "do")
            {
                if (j < tokens.size() && tokens[j].text == "{")
                    loopBody[j] = true;
            }
            else
            {
                // Find opening brace after loop condition
                if (j < tokens.size() && tokens[j].text == "(")
                {
                    int brackets = 1;
                    j++;

                    while (j < tokens.size() && brackets > 0)
                    {
                        if (tokens[j].text == "(")
                            brackets++;
                        else if (tokens[j].text == ")")
                            brackets--;

                        j++;
                    }

                    if (j < tokens.size() && tokens[j].text == "{")
                        loopBody[j] = true;
                }
            }
        }
    }

    for (int i = 0; i < tokens.size(); i++)
    {
        const Token& t = tokens[i];

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

        if (t.text == "{")
        {
            bool isLoop = loopBody[i];
            st.push(isLoop);

            if (isLoop)
            {
                loopDepth++;

                if (loopDepth > maxDepth)
                    maxDepth = loopDepth;
            }
        }
        else if (t.text == "}" && !st.empty())
        {
            if (st.top())
                loopDepth--;

            st.pop();
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

    cout << "\nLoop Nesting Information:\n";
    cout << "Maximum loop depth : " << maxDepth << "\n";
}
