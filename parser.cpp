#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "dsl.h"
#include "debug.h"

// static --------------------------------------------------------------------------------------------------------------

static size_t getFileSize(FILE* file);
static tNode* getGrammar(Data* token);
static tNode* getExpression(Data* token);
static tNode* getMultiplication(Data* token);
static tNode* getDegree(Data* token);
static tNode* getParentheses(Data* token);
static tNode* getNumber(Data* token);
static tNode* getVariable();
static tNode* getFunction(Data* token);
[[noreturn]] static void syntaxError();

// global --------------------------------------------------------------------------------------------------------------

tNode* runParser()
{
    FILE* inputFile = fopen(kNameOfInputFile, "r");
    assert(inputFile);

    Data token = {};

    fread(token.string, sizeof(char), getFileSize(inputFile), inputFile);

    tNode* root = getGrammar(&token);

    FCLOSE(inputFile);

    return root;
}

// static --------------------------------------------------------------------------------------------------------------

static size_t getFileSize(FILE* file)
{
    assert(file);

    long currentPos = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t size = (size_t)ftell(file);
    fseek(file, currentPos, SEEK_SET);

    return size;
}

static tNode* getGrammar(Data* token)
{
    assert(token);

    tNode* node = getExpression(token);
    if (token->string[token->pos] != '$')
    {
        syntaxError();
        token->pos++;
    }
    return node;
}

static tNode* getExpression(Data* token)
{
    assert(token);

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

static tNode* getMultiplication(Data* token)
{
    assert(token);

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

static tNode* getDegree(Data* token)
{
    assert(token);

    tNode* leftNode = getParentheses(token);

    if (token->string[token->pos] == '^')
    {
        token->pos++;
        tNode* rightNode = getParentheses(token);
        leftNode = DEG(leftNode, rightNode);
    }

    return leftNode;
}

static tNode* getParentheses(Data* token)
{
    assert(token);

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
        return getVariable();
    }
    else if (isdigit(token->string[token->pos])) return getNumber(token);
    else if (isalpha(token->string[token->pos])) return getFunction(token);
    else assert(0);
}

static tNode* getFunction(Data* token)
{
    assert(token);

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

static tNode* getNumber(Data* token)
{
    assert(token);

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

static tNode* getVariable()
{
    return VAR(1);
}

static void syntaxError()
{
    fprintf(stderr, "Syntax error\n");

    exit(EXIT_FAILURE);
}
