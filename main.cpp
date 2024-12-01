#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "DSL.h"

#define FREE(ptr_) \
    do { free(ptr_); ptr_ = nullptr; } while(0);
#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = nullptr; } while(0);
#define SYNTAXERROR() \
    do { syntaxError(__LINE__); } while (0);
#define $ fprintf(stderr, "%s:%d in function: %s\n", __FILE__, __LINE__, __func__);

enum NodeType
{
    Number    = 1,
    Variable  = 2,
    Operation = 3,
};

const char* const kNumber = "number";
const char* const kVariable = "variable";
const char* const kOperation = "operation";

enum Operations
{
    NoOperation = 0,
    Add = 1,
    Sub = 2,
    Mul = 3,
    Div = 4,
};

const char* const kAdd = "+";
const char* const kSub = "-";
const char* const kMul = "*";
const char* const kDiv = "/";
const char* const kDeg = "^";

struct tNode
{
    NodeType type;
    int value;
    tNode* left;
    tNode* right;
};

const char* const kDumpFileName = "dump.gv";
const char* const kDiffDumpFileName = "diffDump.gv";

tNode* newNode(NodeType type, int value, tNode* left, tNode* right);
tNode* memoryAllocationForNode(void);
void treeDtor(tNode* node);
void dump(tNode* root, const char* const dumpFileName);
void dumpTreeTraversal(tNode* node, FILE* dumpFile);
void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);
void printOperationType(tNode* node, FILE* dumpFile);
tNode* diff(tNode* node);
tNode* copyNode(tNode* node);

tNode* getP();
tNode* getG();
tNode* getE();
tNode* getT();
tNode* getN();
tNode* getV();
void syntaxError(int line);

const char* s = "5/x$"; // FIXME
int pos = 0;            // FIXME

int main()
{
    tNode* root = getG();

    dump(root, kDumpFileName);

    tNode* diffRoot = diff(root);

    dump(diffRoot, kDiffDumpFileName);

    treeDtor(diffRoot);
    treeDtor(root);

    return 0;
}

tNode* newNode(NodeType type, int value, tNode* left, tNode* right)
{
    tNode* node = nullptr;

    switch (type)
    {
        case Number:
        {
            assert(!left);
            assert(!right);

            node = memoryAllocationForNode();

            node->type = Number;
        }
        break;
        case Variable:
        {
            assert(!left);
            assert(!right);

            node = memoryAllocationForNode();

            node->type = Variable;
        }
        break;
        case Operation:
        {
            assert(left);
            assert(right);

            node = memoryAllocationForNode();

            node->type = Operation;
            node->left = left;
            node->right = right;
        }
        break;
        default: assert(0);
    }

    node->value = value;

    return node;
}

tNode* memoryAllocationForNode(void)
{
    tNode* node = (tNode*)calloc(1, sizeof(tNode));
    assert(node);

    return node;
}

void treeDtor(tNode* node)
{
    assert(node);

    if (node->left) treeDtor(node->left);
    if (node->right) treeDtor(node->right);

    memset(node, 0, sizeof(tNode));
    FREE(node);
}

void dump(tNode* root, const char* const dumpFileName)
{
    assert(root);

    FILE* dumpFile = fopen(dumpFileName, "w");
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n");
    fprintf(dumpFile, "{\n    ");
    fprintf(dumpFile, "rankdir = TB;\n    ");
    fprintf(dumpFile, "node [shape=record,style = rounded,color=\"#252A34\",penwidth = 2.5];\n    ");
    fprintf(dumpFile, "bgcolor = \"#E7ECEF\";\n\n");

    dumpTreeTraversal(root, dumpFile);
    dumpTreeTraversalWithArrows(root, dumpFile);

    fprintf(dumpFile, "}\n");

    FCLOSE(dumpFile);

    (!strcmp(dumpFileName, kDumpFileName)) ? system("dot dump.gv -Tpng -o dump.png")
                                           : system("dot diffDump.gv -Tpng -o diffDump.png");
}

void dumpTreeTraversal(tNode* node, FILE* dumpFile)
{
    assert(dumpFile);
    if (!node) return;

    static size_t rank = 0;
    fprintf(dumpFile, "    node_%p [rank=%lu,label=\" { node: %p", node, rank, node);

    if (node->type == Number)
    {
        fprintf(dumpFile, " | type: %s | value: %d | ", kNumber, node->value);
    }
    if (node->type == Variable)
    {
        fprintf(dumpFile, " | type: %s | value: %d | ", kVariable, node->value);
    }
    if (node->type == Operation)
    {
        fprintf(dumpFile, " | type: %s | value: ", kOperation);
        printOperationType(node, dumpFile);
        fprintf(dumpFile, " | ");
    }

    fprintf(dumpFile, "{ left: %p | right: %p }} \"];\n", node->left, node->right);

    if (node->left)
    {
        rank++;
        dumpTreeTraversal(node->left, dumpFile);
    }
    if (node->right)
    {
        rank++;
        dumpTreeTraversal(node->right, dumpFile);
    }
    rank--;
}

void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile)
{
    assert(dumpFile);
    if (!node) return;

    static int flag = 0;
    if (node->left)
    {
        (flag++) ? fprintf(dumpFile, "-> node_%p ", node->left)
                 : fprintf(dumpFile, "    node_%p -> node_%p ", node, node->left);
        dumpTreeTraversalWithArrows(node->left, dumpFile);
    }
    if (node->right)
    {
        (flag++) ? fprintf(dumpFile, "-> node_%p ", node->right)
                 : fprintf(dumpFile, "    node_%p -> node_%p ", node, node->right);
        dumpTreeTraversalWithArrows(node->right, dumpFile);
    }
    if (flag) { fprintf(dumpFile, ";\n"); }
    flag = 0;
}

void printOperationType(tNode* node, FILE* dumpFile) // NOTE define maybe ?
{
    assert(node);
    assert(node->type == Operation);

    switch (node->value)
    {
        case Add: fprintf(dumpFile, "%s", kAdd); break;
        case Sub: fprintf(dumpFile, "%s", kSub); break;
        case Mul: fprintf(dumpFile, "%s", kMul); break;
        case Div: fprintf(dumpFile, "%s", kDiv); break;
        default: assert(0);
    }
}

tNode* getG()
{
    tNode* node = getE();
    if (s[pos] != '$')
    {
        SYNTAXERROR();
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
    tNode* leftNode = getP();

    while (s[pos] == '*' || s[pos] == '/')
    {
        int op = s[pos];
        pos++;
        tNode* rightNode = getP();
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

tNode* getP()
{
    if (s[pos] == '(')
    {
        pos++;
        tNode* node = getE();
        if (s[pos] != ')')
            SYNTAXERROR();
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

tNode* getV()
{
    return newNode(Variable, 1, NULL, NULL);
}

tNode* getN()
{
    int val = 0;
    int old_pos = pos;
    while ('0' <= s[pos] && s[pos] <= '9')
    {
        val = val * 10 + s[pos] - '0';
        pos++;
    }
    if (old_pos == pos)
        SYNTAXERROR();

    return newNode(Number, val, NULL, NULL);
}

void syntaxError(int line)
{
    fprintf(stderr, "SyntaxError: %d\n", line);
    exit(0);
}

tNode* diff(tNode* node)
{
    if (node->type == Number   ) return newNode(Number, 0, NULL, NULL);
    if (node->type == Variable ) return newNode(Number, 1, NULL, NULL);
    if (node->type == Operation)
    {
        switch (node->value)
        {
            case Add:
            {
                return _ADD(diff(node->left), diff(node->right));
            }
            break;
            case Sub:
            {
                return _SUB(diff(node->left), diff(node->right));
            }
            break;
            case Mul:
            {
                return _ADD(
                            _MUL(diff(node->left ), copyNode(node->right)),
                            _MUL(diff(node->right), copyNode(node->left)));
            }
            break;
            case Div:
            {
                return _DIV(
                            _SUB(
                                 _MUL(diff(node->left ), copyNode(node->right)),
                                 _MUL(diff(node->right), copyNode(node->left))),
                            _MUL(copyNode(node->right), copyNode(node->right)));
            }
            break;
            default: assert(0);
        }
    }
}

tNode* copyNode(tNode* node)
{
    return newNode(node->type, node->value, node->left, node->right);
}
