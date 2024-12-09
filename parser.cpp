#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "dsl.h"
#include "debug.h"

const char* s = "55*cos(x)+28*x^2$"; /*10*x^2+20*x+333$";*/ // FIXME add file
size_t pos = 0; // FIXME

#define syntaxError() SyntaxError(__LINE__) // FIXME

tNode* getGeneral()
{
    tNode* node = getExpression();
    if (s[pos] != '$')
    {
        syntaxError();
        pos++;
    }
    return node;
}

tNode* getExpression()
{
    tNode* leftNode = getMultiplication();

    while (s[pos] == '+' || s[pos] == '-')
    {
        int op = s[pos];
        pos++;
        tNode* rightNode = getMultiplication();
        if (op == '+')
        {
            leftNode = ADD(leftNode, rightNode);
        }
        else
        {
            leftNode = SUB(leftNode, rightNode);
        }
    }
    return leftNode;
}

tNode* getMultiplication()
{
    tNode* leftNode = getDegree();

    while (s[pos] == '*' || s[pos] == '/')
    {
        int op = s[pos];
        pos++;
        tNode* rightNode = getDegree();
        if (op == '*')
        {
            leftNode = MUL(leftNode, rightNode);
        }
        else
        {
            leftNode = DIV(leftNode, rightNode);
        }
    }
    return leftNode;
}

tNode* getDegree()
{
    tNode* leftNode = getParentheses();

    if (s[pos] == '^')
    {
        pos++;
        tNode* rightNode = getParentheses();
        leftNode = DEG(leftNode, rightNode);
    }

    return leftNode;
}

tNode* getParentheses()
{
    if (s[pos] == '(')
    {
        pos++;
        tNode* node = getExpression();
        if (s[pos] != ')')
            syntaxError();
        pos++;
        return node;
    }
    else if (s[pos] == 'x')
    {
        pos++;
        return getVariable();
    }
    else if (isdigit(s[pos])) return getNumber();
    else if (isalpha(s[pos])) return getFunction();
    else assert(0);
}

tNode* getFunction()
{
    char* word = (char*)calloc(32, sizeof(char)); //FIXME
    assert(word);

    sscanf(s+pos, "%[a-z]", word);

    if (!strcmp(word, kLn)) // FIXME copypaste
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Ln, getExpression(), NULL);
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else if (!strcmp(word, kLog))
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* leftNode = getExpression();
        if (s[pos] != ')') syntaxError();
        pos++;
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Log, leftNode, getExpression());
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else if (!strcmp(word, kLg))
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Lg, getExpression(), NULL);
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else if (!strcmp(word, kSin))
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Sin, getExpression(), NULL);
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else if (!strcmp(word, kCos))
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Cos, getExpression(), NULL);
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else if (!strcmp(word, kTg))
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Tg, getExpression(), NULL);
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else if (!strcmp(word, kCtg))
    {
        pos += strlen(word);
        FREE(word);
        if (s[pos] != '(') syntaxError();
        pos++;
        tNode* node = newNode(Operation, Ctg, getExpression(), NULL);
        if (s[pos] != ')') syntaxError();
        pos++;
        return node;
    }
    else
    {
        FREE(word);
        syntaxError();
    }
}

tNode* getNumber()
{
    int value = 0;
    size_t old_pos = pos;
    while (isdigit(s[pos]))
    {
        value = value * 10 + s[pos] - '0';
        pos++;
    }
    if (old_pos == pos)
        syntaxError();

    return NUM(value);
}

tNode* getVariable()
{
    return VAR(1);
}

void SyntaxError(int lines)
{
    fprintf(stderr, "Syntax error: %d\n", lines);

    exit(EXIT_FAILURE);
}
