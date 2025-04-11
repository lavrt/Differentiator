#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

// static --------------------------------------------------------------------------------------------------------------

static tNode* memoryAllocationForNode();
static void dumpTreeTraversal(tNode* node, FILE* dumpFile);
static void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);
static void printOperationType(tNode* node, FILE* dumpFile);

// global --------------------------------------------------------------------------------------------------------------

tNode* newNode(NodeType type, int value, tNode* left, tNode* right)
{
    tNode* node = NULL;

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
    fprintf(dumpFile, "node [shape=record,style = filled,penwidth = 2.5];\n    ");
    fprintf(dumpFile, "bgcolor = \"#FDFBE4\";\n\n");

    dumpTreeTraversal(root, dumpFile);
    dumpTreeTraversalWithArrows(root, dumpFile);

    fprintf(dumpFile, "}\n");

    FCLOSE(dumpFile);

    #if defined GRAPHVIZ
        if (!strcmp(dumpFileName, kDumpFileName))
            system("dot dump.gv -Tpng -o dump.png");
        else if (!strcmp(dumpFileName, kFirstDerivativeDumpFileName))
            system("dot dumpFirstDerivative.gv -Tpng -o dumpFirstDerivative.png");
        else
            system("dot dumpSecondDerivative.gv -Tpng -o dumpSecondDerivative.png");
    #endif
}

tNode* copyNode(tNode* node)
{
    return (node)
                  ? newNode(node->type, node->value, copyNode(node->left), copyNode(node->right))
                  : NULL;
}

bool subtreeContainsVariable(tNode* node)
{
    if (!node) return false;

    static int rank = 0;
    static bool presenceOfVariable = false;

    if (node->type == Variable)
    {
        presenceOfVariable = true;
    }
    else
    {
        rank++;
        subtreeContainsVariable(node->left);
        subtreeContainsVariable(node->right);
        rank--;
    }

    if (presenceOfVariable && !rank)
    {
        presenceOfVariable = false;
        return true;
    }
    else
    {
        return false;
    }
}

// static --------------------------------------------------------------------------------------------------------------

static tNode* memoryAllocationForNode(void)
{
    tNode* node = (tNode*)calloc(1, sizeof(tNode));
    assert(node);

    return node;
}

static void dumpTreeTraversal(tNode* node, FILE* dumpFile)
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

    fprintf(dumpFile, "{ left: %p | right: %p }} \"", node->left, node->right);

    if (node->type == Number)
    {
        fprintf(dumpFile, ", color = \"#DBD4FF\"];\n");
    }
    if (node->type == Variable)
    {
        fprintf(dumpFile, ", color = \"#EBAEE6\"];\n");
    }
    if (node->type == Operation)
    {
        fprintf(dumpFile, ", color = \"#E8D59E\"];\n");
    }

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

static void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile)
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

static void printOperationType(tNode* node, FILE* dumpFile)
{
    assert(node);
    assert(node->type == Operation);

    switch (node->value)
    {
        case Add: fprintf(dumpFile, "%s", kAdd); break;
        case Sub: fprintf(dumpFile, "%s", kSub); break;
        case Mul: fprintf(dumpFile, "%s", kMul); break;
        case Div: fprintf(dumpFile, "%s", kDiv); break;
        case Deg: fprintf(dumpFile, "%s", kDeg); break;
        case Ln : fprintf(dumpFile, "%s", kLn ); break;
        case Log: fprintf(dumpFile, "%s", kLog); break;
        case Lg : fprintf(dumpFile, "%s", kLg ); break;
        case Sin: fprintf(dumpFile, "%s", kSin); break;
        case Cos: fprintf(dumpFile, "%s", kCos); break;
        case Tg : fprintf(dumpFile, "%s", kTg ); break;
        case Ctg: fprintf(dumpFile, "%s", kCtg); break;
        case uSub:fprintf(dumpFile, "%s", kSub); break;

        default: assert(0);
    }
}
