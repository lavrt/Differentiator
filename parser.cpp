#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "dsl.h"
#include "debug.h"

tNode* runParser()
{
    FILE* inputFile = fopen(kNameOfInputFile, "r");
    assert(inputFile);

    Data token = {};

    fread(token.string, sizeof(char), getFileSize(inputFile), inputFile);

    tNode* diffRoot = getGrammar(&token);

    FCLOSE(inputFile);

    return diffRoot;
}

size_t getFileSize(FILE* file)
{
    assert(file);

    long currentPos = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t size = (size_t)ftell(file);
    fseek(file, currentPos, SEEK_SET);

    return size;
}

tNode* getGrammar(Data* token)
{
    tNode* node = getExpression(token);
    if (token->string[token->pos] != '$')
    {
        syntaxError();
        token->pos++;
    }
    return node;
}

tNode* getExpression(Data* token)
{
    tNode* leftNode = getMultiplication(token);

    while (token->string[token->pos] == '+' || token->string[token->pos] == '-')
    {
        int op = token->string[token->pos];
        token->pos++;
        tNode* rightNode = getMultiplication(token);
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

tNode* getMultiplication(Data* token)
{
    tNode* leftNode = getDegree(token);

    while (token->string[token->pos] == '*' || token->string[token->pos] == '/')
    {
        int op = token->string[token->pos];
        token->pos++;
        tNode* rightNode = getDegree(token);
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

tNode* getDegree(Data* token)
{
    tNode* leftNode = getParentheses(token);

    if (token->string[token->pos] == '^')
    {
        token->pos++;
        tNode* rightNode = getParentheses(token);
        leftNode = DEG(leftNode, rightNode);
    }

    return leftNode;
}

tNode* getParentheses(Data* token)
{
    if (token->string[token->pos] == '(')
    {
        token->pos++;
        tNode* node = getExpression(token);
        if (token->string[token->pos] != ')')
            syntaxError();
        token->pos++;
        return node;
    }
    else if (token->string[token->pos] == 'x')
    {
        token->pos++;
        return getVariable(token);
    }
    else if (isdigit(token->string[token->pos])) return getNumber(token);
    else if (isalpha(token->string[token->pos])) return getFunction(token);
    else assert(0);
}

tNode* getFunction(Data* token)
{
    char* word = (char*)calloc(kBufferSize, sizeof(char));
    assert(word);

    sscanf(token->string + token->pos, "%[a-z]", word);

    if (!strcmp(word, kLn))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Ln, getExpression(token), NULL);
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else if (!strcmp(word, kLog))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* leftNode = getExpression(token);
        CHECK_RIGHT_PARENTHESIS;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Log, leftNode, getExpression(token));
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else if (!strcmp(word, kLg))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Lg, getExpression(token), NULL);
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else if (!strcmp(word, kSin))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Sin, getExpression(token), NULL);
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else if (!strcmp(word, kCos))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Cos, getExpression(token), NULL);
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else if (!strcmp(word, kTg))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Tg, getExpression(token), NULL);
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else if (!strcmp(word, kCtg))
    {
        MOVE_POS_BY_LENGTH_OF_WORD;

        CHECK_LEFT_PARENTHESIS;
        tNode* node = newNode(Operation, Ctg, getExpression(token), NULL);
        CHECK_RIGHT_PARENTHESIS;

        return node;
    }
    else
    {
        FREE(word);
        syntaxError();
    }
}

tNode* getNumber(Data* token)
{
    int value = 0;
    size_t old_pos = token->pos;
    while (isdigit(token->string[token->pos]))
    {
        value = value * 10 + token->string[token->pos] - '0';
        token->pos++;
    }
    if (old_pos == token->pos)
        syntaxError();

    return NUM(value);
}

tNode* getVariable(Data* token)
{
    return VAR(1);
}

void syntaxError()
{
    fprintf(stderr, "Syntax error\n");

    exit(EXIT_FAILURE);
}
