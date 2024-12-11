#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "node.h"

const size_t kBufferSize = 32;
const size_t kInputStringSize = 128;

const char* const kNameOfInputFile = "expression.txt";

struct Data
{
    char string[kInputStringSize];
    size_t pos;
};
#define MOVE_POS_BY_LENGTH_OF_WORD \
    do { token->pos += strlen(word); FREE(word); } while (0);
#define CHECK_LEFT_PARENTHESIS \
    do { if (token->string[token->pos++] != '(') syntaxError(); } while(0);
#define CHECK_RIGHT_PARENTHESIS \
    do { if (token->string[token->pos++] != ')') syntaxError(); } while(0);

tNode* runParser();
size_t getFileSize(FILE* file);

tNode* getGrammar(Data* token);
tNode* getExpression(Data* token);
tNode* getMultiplication(Data* token);
tNode* getDegree(Data* token);
tNode* getParentheses(Data* token);
tNode* getNumber(Data* token);
tNode* getVariable(Data* token);
tNode* getFunction(Data* token);
[[noreturn]] void syntaxError();

#endif // PARSER_H
