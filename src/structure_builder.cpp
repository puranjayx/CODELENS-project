
#include "structure_builder.h"

// Check whether a token starts a control structure
bool isStructure(const string& word)
{
    return word == "for" ||
           word == "while" ||
           word == "if" ||
           word == "else" ||
           word == "do" ||
           word == "switch";
}

// Convert token into node type
NodeType getNodeType(const string& word)
{
    if (word == "for")
        return NodeType::ForLoop;

    if (word == "while")
        return NodeType::WhileLoop;

    if (word == "do")
        return NodeType::DoWhileLoop;

    if (word == "if")
        return NodeType::IfStatement;

    if (word == "else")
        return NodeType::ElseStatement;

    if (word == "switch")
        return NodeType::SwitchStatement;

    return NodeType::Statement;
}

// Check whether the tokens form a function definition
bool isFunctionDefinition(const vector<Token>& tokens, int i)
{
    int j = i;
    int parens = 0;
    bool foundParentheses = false;

    while (j < (int)tokens.size())
    {
        if (tokens[j].text == "(")
        {
            parens++;
            foundParentheses = true;
        }
        else if (tokens[j].text == ")")
        {
            parens--;

            if (parens < 0)
                return false;
        }
        else if (tokens[j].text == ";" && parens == 0)
        {
            return false;
        }
        else if (tokens[j].text == "{" && parens == 0)
        {
            // A function signature should have parentheses
            // and a name immediately before the opening parenthesis.
            if (!foundParentheses)
                return false;

            int openParen = -1;

            for (int k = i; k < j; k++)
            {
                if (tokens[k].text == "(")
                {
                    openParen = k;
                    break;
                }
            }

            if (openParen <= i)
                return false;

            if (tokens[openParen - 1].type != TokType::Identifier)
                return false;

            // Reject control structures and common non-function
            // expressions that can also contain parentheses.
            if (isStructure(tokens[i].text))
                return false;

            return true;
        }

        j++;
    }

    return false;
}

// Read one statement, function, structure, or block
SyntaxNode readNode(const vector<Token>& tokens, int& i)
{
    SyntaxNode node;

    if (i >= (int)tokens.size())
        return node;

    node.line = tokens[i].line;

    // Handle loops and conditional structures first
    if (isStructure(tokens[i].text))
    {
        node.type = getNodeType(tokens[i].text);
        node.tokens.push_back(tokens[i]);
        i++;

        // Store the condition inside parentheses
        if (i < (int)tokens.size() && tokens[i].text == "(")
        {
            int brackets = 0;

            do
            {
                if (tokens[i].text == "(")
                    brackets++;

                if (tokens[i].text == ")")
                    brackets--;

                node.tokens.push_back(tokens[i]);
                i++;

            } while (i < (int)tokens.size() && brackets > 0);
        }

        // Read the body
        if (i < (int)tokens.size() && tokens[i].text == "{")
        {
            i++;

            while (i < (int)tokens.size() && tokens[i].text != "}")
            {
                int oldI = i;
                node.children.push_back(readNode(tokens, i));

                // Prevent an infinite loop if parsing cannot advance
                if (i == oldI)
                    i++;
            }

            if (i < (int)tokens.size() && tokens[i].text == "}")
                i++;
        }
        else if (i < (int)tokens.size() && tokens[i].text != "}")
        {
            // Handle a body without braces
            node.children.push_back(readNode(tokens, i));
        }

        // Consume the semicolon after do-while
        if (node.type == NodeType::DoWhileLoop &&
            i < (int)tokens.size() && tokens[i].text == "while")
        {
            while (i < (int)tokens.size())
            {
                string word = tokens[i].text;
                node.tokens.push_back(tokens[i]);
                i++;

                if (word == ";")
                    break;
            }
        }

        return node;
    }

    // Handle function definitions
    if (isFunctionDefinition(tokens, i))
    {
        node.type = NodeType::Function;

        // Store the function signature
        int parens = 0;

        while (i < (int)tokens.size())
        {
            if (tokens[i].text == "(")
                parens++;

            if (tokens[i].text == ")")
                parens--;

            if (tokens[i].text == "{" && parens == 0)
                break;

            node.tokens.push_back(tokens[i]);
            i++;
        }

        // Read the function body
        if (i < (int)tokens.size() && tokens[i].text == "{")
        {
            i++;

            while (i < (int)tokens.size() && tokens[i].text != "}")
            {
                int oldI = i;
                node.children.push_back(readNode(tokens, i));

                if (i == oldI)
                    i++;
            }

            if (i < (int)tokens.size() && tokens[i].text == "}")
                i++;
        }

        return node;
    }

    // Handle a normal block
    if (tokens[i].text == "{")
    {
        node.type = NodeType::Block;
        i++;

        while (i < (int)tokens.size() && tokens[i].text != "}")
        {
            int oldI = i;
            node.children.push_back(readNode(tokens, i));

            if (i == oldI)
                i++;
        }

        if (i < (int)tokens.size() && tokens[i].text == "}")
            i++;

        return node;
    }

    // Handle a normal statement until semicolon
    node.type = NodeType::Statement;

    while (i < (int)tokens.size())
    {
        if (tokens[i].text == "{" || tokens[i].text == "}")
            break;

        node.tokens.push_back(tokens[i]);

        if (tokens[i].text == ";")
        {
            i++;
            break;
        }

        i++;
    }

    return node;
}

// Build the complete syntax tree
SyntaxNode buildSyntaxTree(const vector<Token>& tokens)
{
    SyntaxNode root;
    root.type = NodeType::Program;

    int i = 0;

    while (i < (int)tokens.size())
    {
        int oldI = i;
        root.children.push_back(readNode(tokens, i));

        if (i == oldI)
            i++;
    }

    return root;
}

// Print the syntax tree
void printSyntaxTree(const SyntaxNode& node, int depth)
{
    for (int i = 0; i < depth; i++)
        cout << "  ";

    switch (node.type)
    {
        case NodeType::Program:
            cout << "Program";
            break;

        case NodeType::Function:
            cout << "Function";

            // Print function name/signature
            for (const Token& t : node.tokens)
                cout << " " << t.text;

            break;

        case NodeType::ForLoop:
            cout << "For Loop";
            break;

        case NodeType::WhileLoop:
            cout << "While Loop";
            break;

        case NodeType::DoWhileLoop:
            cout << "Do-While Loop";
            break;

        case NodeType::IfStatement:
            cout << "If";
            break;

        case NodeType::ElseStatement:
            cout << "Else";
            break;

        case NodeType::SwitchStatement:
            cout << "Switch";
            break;

        case NodeType::Block:
            cout << "Block";
            break;

        case NodeType::Statement:
            cout << "Statement";
            break;
    }

    cout << " (line " << node.line << ")\n";

    for (const SyntaxNode& child : node.children)
        printSyntaxTree(child, depth + 1);
}
