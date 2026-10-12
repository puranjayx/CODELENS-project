
#pragma once

#include "types.h"

SyntaxNode buildSyntaxTree(const vector<Token>& tokens);
void printSyntaxTree(const SyntaxNode& node, int depth = 0);