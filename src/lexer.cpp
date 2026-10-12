
#include "lexer.h"
#include <cctype>
#include <unordered_set>

using namespace std;

vector<Token> tokenize(const string& src) {
    vector<Token> tokens;
    int line = 1;
    size_t i = 0;
    size_t n = src.size();

    static const unordered_set<string> keywords = {
        "int", "float", "double", "char", "bool", "void",
        "for", "while", "do", "if", "else", "switch", "case",
        "break", "continue", "return", "using", "namespace",
        "class", "struct", "new", "delete", "const", "auto",
        "long", "short", "unsigned", "true", "false"
    };

    static const unordered_set<string> multiOps = {
        "==", "!=", "<=", ">=", "&&", "||", "++", "--",
        "+=", "-=", "*=", "/=", "<<", ">>", "->", "::"
    };

    static const string singleOps = "+-*/%=<>!&|^~?";

    while (i < n) {
        if (src[i] == '\n') {
            line++;
            i++;
            continue;
        }

        if (isspace(static_cast<unsigned char>(src[i]))) {
            i++;
            continue;
        }

        if (src[i] == '#') {
            while (i < n && src[i] != '\n') {
                i++;
            }
            continue;
        }

        if (src[i] == '/' && i + 1 < n && src[i + 1] == '/') {
            while (i < n && src[i] != '\n') {
                i++;
            }
            continue;
        }

        if (src[i] == '/' && i + 1 < n && src[i + 1] == '*') {
            i += 2;
            while (i < n && !(src[i] == '*' && i + 1 < n && src[i + 1] == '/')) {
                if (src[i] == '\n') {
                    line++;
                }
                i++;
            }
            if (i < n) {
                i += 2;
            }
            continue;
        }

        if (src[i] == '"') {
            i++;
            while (i < n && src[i] != '"') {
                if (src[i] == '\\' && i + 1 < n) {
                    if (src[i + 1] == '\n') line++;
                    i += 2;
                } else {
                    if (src[i] == '\n') line++;
                    i++;
                }
            }
            if (i < n) {
                i++;
            }
            continue;
        }

        if (src[i] == '\'') {
            i++;
            while (i < n && src[i] != '\'') {
                if (src[i] == '\\' && i + 1 < n) {
                    if (src[i + 1] == '\n') line++;
                    i += 2;
                } else {
                    if (src[i] == '\n') line++;
                    i++;
                }
            }
            if (i < n) {
                i++;
            }
            continue;
        }

        if (isalpha(static_cast<unsigned char>(src[i])) || src[i] == '_') {
            size_t start = i;
            while (i < n && (isalnum(static_cast<unsigned char>(src[i])) || src[i] == '_')) {
                i++;
            }
            string text = src.substr(start, i - start);
            TokType type = keywords.count(text) ? TokType::Keyword : TokType::Identifier;
            tokens.push_back({type, text, line});
            continue;
        }

        if (isdigit(static_cast<unsigned char>(src[i]))) {
            size_t start = i;
            while (i < n && isdigit(static_cast<unsigned char>(src[i]))) {
                i++;
            }
            if (i < n && src[i] == '.') {
                i++;
                while (i < n && isdigit(static_cast<unsigned char>(src[i]))) {
                    i++;
                }
            }
            tokens.push_back({TokType::Number, src.substr(start, i - start), line});
            continue;
        }

        if (i + 1 < n) {
            string op2 = src.substr(i, 2);
            if (multiOps.count(op2)) {
                tokens.push_back({TokType::Operator, op2, line});
                i += 2;
                continue;
            }
        }

        if (singleOps.find(src[i]) != string::npos) {
            tokens.push_back({TokType::Operator, string(1, src[i]), line});
            i++;
            continue;
        }

        char c = src[i];
        if (c == '(' || c == ')' || c == '{' || c == '}' ||
            c == '[' || c == ']' || c == ';' || c == ',' ||
            c == '.' || c == ':') {
            tokens.push_back({TokType::Symbol, string(1, c), line});
            i++;
            continue;
        }

        i++;
    }

    return tokens;
}
