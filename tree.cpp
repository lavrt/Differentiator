#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

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
    fprintf(dumpFile, "node [shape=record,style = filled,penwidth = 2.5];\n    ");
    fprintf(dumpFile, "bgcolor = \"#27a300\";\n\n");

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

    fprintf(dumpFile, "{ left: %p | right: %p }} \"", node->left, node->right);

    if (node->type == Number)
    {
        fprintf(dumpFile, ", color = \"#fbf8cc\"];\n");
    }
    if (node->type == Variable)
    {
        fprintf(dumpFile, ", color = \"#f1c0e8\"];\n");
    }
    if (node->type == Operation)
    {
        fprintf(dumpFile, ", color = \"#b9fbc0\"];\n");
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
        case Deg: fprintf(dumpFile, "%s", kDeg); break;
        case Ln : fprintf(dumpFile, "%s", kLn ); break;
        case Sin: fprintf(dumpFile, "%s", kSin); break;
        case Cos: fprintf(dumpFile, "%s", kCos); break;
        case Tg : fprintf(dumpFile, "%s", kTg ); break;
        case Ctg: fprintf(dumpFile, "%s", kCtg); break;

        default: assert(0);
    }
}

tNode* copyNode(tNode* node)
{
    return (node)
                  ? newNode(node->type, node->value, copyNode(node->left), copyNode(node->right))
                  : nullptr;
}

bool subtreeContainsVariable(tNode* node)
{
    if (!node) return false;

    static bool presenceOfVariable = false;

    if (node->type == Variable)
    {
        presenceOfVariable = true;
    }
    else
    {
        subtreeContainsVariable(node->left);
        subtreeContainsVariable(node->right);
    }

    if (presenceOfVariable)
    {
        presenceOfVariable = false;
        return true;
    }
    else
    {
        return false;
    }
}
