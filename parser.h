#ifndef PARSER_H
#define PARSER_H

#include "node.h"

tNode* getGeneral();
tNode* getExpression();
tNode* getMultiplication();
tNode* getDegree();
tNode* getParentheses();
tNode* getNumber();
tNode* getVariable();
tNode* getFunction();
[[noreturn]] void SyntaxError(int lines);

#endif // PARSER_H
