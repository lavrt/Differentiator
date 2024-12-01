#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "dsl.h"
#include "debug.h"

const char* s = "x^3$"; // FIXME
int pos = 0; // FIXME

tNode* getG()
{
    tNode* node = getE();
    if (s[pos] != '$')
    {
        syntaxError();
        pos++;
    }
    return node;
}

tNode* getE()
{
    tNode* leftNode = getT();

    while (s[pos] == '+' || s[pos] == '-')
    {
        int op = s[pos];
        pos++;
        tNode* rightNode = getT();
        if (op == '+')
        {
            leftNode = _ADD(leftNode, rightNode);
        }
        else
        {
            leftNode = _SUB(leftNode, rightNode);
        }
    }
    return leftNode;
}

tNode* getT()
{
    tNode* leftNode = getD();

    while (s[pos] == '*' || s[pos] == '/')
    {
        int op = s[pos];
        pos++;
        tNode* rightNode = getD();
        if (op == '*')
        {
            leftNode = _MUL(leftNode, rightNode);
        }
        else
        {
            leftNode = _DIV(leftNode, rightNode);
        }
    }
    return leftNode;
}

tNode* getD()
{
    tNode* leftNode = getP();

    if (s[pos] == '^')
    {
        pos++;
        tNode* rightNode = getP();
        leftNode = _DEG(leftNode, rightNode);
    }
    // else if ()

    return leftNode;
}

tNode* getP()
{
    if (s[pos] == '(')
    {
        pos++;
        tNode* node = getE();
        if (s[pos] != ')')
            syntaxError();
        pos++;
        return node;
    }
    else if (s[pos] == 'x')
    {
        pos++;
        return getV();
    }
    else return getN();
}

tNode* getN()
{
    int value = 0;
    int old_pos = pos;
    while ('0' <= s[pos] && s[pos] <= '9')
    {
        value = value * 10 + s[pos] - '0';
        pos++;
    }
    if (old_pos == pos)
        syntaxError();

    return _NUM(value);
}

tNode* getV()
{
    return _VAR(1);
}

void syntaxError()
{
    fprintf(stderr, "Syntax error\n");
    exit(0);
}
