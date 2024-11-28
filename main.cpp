#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define FREE(ptr_) \
    do { free(ptr_); ptr_ = nullptr; } while(0);
#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = nullptr; } while(0);
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

struct tNode
{
    NodeType type;
    int value;
    tNode* left;
    tNode* right;
};

struct infoNode
{
    char* data; // FIXME const
    int rank;
    int sequenceNumber;
};

const char* const kDumpFileName = "dump.gv";

tNode* newNode(NodeType type, int value, tNode* left, tNode* right);
tNode* memoryAllocationForNode(void);
void treeDtor(tNode* node);
void dump(tNode* root);
void dumpTreeTraversal(tNode* node, FILE* dumpFile);
void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);
void printOperationType(tNode* node, FILE* dumpFile);
void writingExpressionToFile(tNode* root);
void goToSubtreeWriting(tNode* node, FILE* expressionFile);
void readingExpressionFromFile(tNode* root);
void goToSubtreeReading(tNode* node, char* dataArray, int64_t fileSize);
bool isOperation(char symbol);
int compareNodesByRank(const void* firstNode, const void* secondNode);
int compareNodesOfSameRankInOrder(const void* firstNode, const void* secondNode);

int main()
{
    tNode* root = newNode(Operation, Div,
        newNode(Operation, Add, newNode(Operation, Mul, newNode(Number, 3, NULL, NULL), newNode(Operation, Sub, newNode(Number, 10, NULL, NULL), newNode(Number, 1, NULL, NULL))), newNode(Variable, 1, NULL, NULL)),
        newNode(Operation, Sub, newNode(Number, 1000, NULL, NULL), newNode(Number, 7, NULL, NULL)));

    dump(root);
    // tNode* root = memoryAllocationForNode();

    fprintf(stderr, "%p\n", root);

    writingExpressionToFile(root);

    readingExpressionFromFile(root);

    dump(root);





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

void dump(tNode* root)
{
    assert(root);

    FILE* dumpFile = fopen(kDumpFileName, "w");
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

    system("dot dump.gv -Tpng -o dump.png");
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

void printOperationType(tNode* node, FILE* dumpFile)
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

void writingExpressionToFile(tNode* root)
{
    FILE* expressionFile = fopen("expression.txt", "w"); // FIXME const

    goToSubtreeWriting(root, expressionFile);

    FCLOSE(expressionFile);
}

void goToSubtreeWriting(tNode* node, FILE* expressionFile)
{
    if (!node->left && !node->right)
    {
        fprintf(expressionFile, "(%d)", node->value);
        return;
    }

    fprintf(expressionFile, "(");

    goToSubtreeWriting(node->left, expressionFile);

    if (node->type == Operation)
    {
        // fprintf(expressionFile, "(");
        printOperationType(node, expressionFile);
        // fprintf(expressionFile, ")");
    }

    goToSubtreeWriting(node->right, expressionFile);

    fprintf(expressionFile, ")");
}

void readingExpressionFromFile(tNode* root)
{
    FILE* expressionFile = fopen("expression.txt", "r"); // FIXME const, assert

    fseek(expressionFile, 0, SEEK_END);
    int64_t fileSize = ftello(expressionFile);
    fseek(expressionFile, 0, SEEK_SET);

    char* dataArray = (char*)calloc((size_t)fileSize + 1, sizeof(char));
    assert(dataArray);
    fread(dataArray, sizeof(char), (size_t)fileSize, expressionFile);

    for (int i = 0; i < fileSize; i++) printf("%c", dataArray[i]); // NOTE DEBUG

    // char* ptr = nullptr;
    infoNode* dataArrayClear = (infoNode*)calloc(1, sizeof(infoNode));
    int index = 0;

    int rank = 0;
    bool firstUseOfStrtokFunction = true;

    printf("\n"); // NOTE DEBUG

    for (int i = 0; i < fileSize; i++)
    {
        if (dataArray[i] == '\0') { rank++; continue; }
        if      (dataArray[i] == '(') { rank++;  }
        else if (dataArray[i] == ')') { rank--;  }
        else
        {
            dataArrayClear = (infoNode*)realloc(dataArrayClear, (index + 1) * sizeof(infoNode));
            assert(dataArrayClear);

            dataArrayClear[index].rank = rank;
            dataArrayClear[index].sequenceNumber = index + 1;

            index++;
            while (dataArray[i] != '(' && dataArray[i] != ')' && i < fileSize && dataArray[i] != '\0') i++;
            i--;
        }
    }
printf("\n"); // NOTE DEBUG
    // for (int j = 0; j < index; j++) printf("_%s_%d_", dataArrayClear[j].data, dataArrayClear[j].rank); // NOTE DEBUG


    char* ptr = nullptr;
    for (int i = 0; i < index; i++)
    {
        dataArrayClear[i].data = (firstUseOfStrtokFunction) ? strtok(dataArray, "()") : strtok(NULL, "()");
        firstUseOfStrtokFunction = false;
    }

    qsort(dataArrayClear, index, sizeof(infoNode), compareNodesByRank);
    qsort(dataArrayClear, index, sizeof(infoNode), compareNodesOfSameRankInOrder);
for (int j = 0; j < index; j++) printf("%7d", dataArrayClear[j].rank); // NOTE DEBUG
printf("\n");
    for (int j = 0; j < index; j++) printf("%7s", dataArrayClear[j].data, dataArrayClear[j].rank); // NOTE DEBUG
printf("\n");
    for (int j = 0; j < index; j++) printf("%7d", dataArrayClear[j].sequenceNumber);

    goToSubtreeReading(root, dataArray, fileSize);

    FREE(dataArrayClear);
    FREE(dataArray);
    FCLOSE(expressionFile);
}

bool isOperation(char symbol)
{
    char tmp[8] = {}; tmp[0] = symbol; tmp[1] = '\0';  fprintf(stderr, "_%s_", tmp);
         if (!strcmp(tmp, kAdd)) return true;
    else if (!strcmp(tmp, kSub)) return true;
    else if (!strcmp(tmp, kMul)) return true;
    else if (!strcmp(tmp, kDiv)) return true;
    else                         return false;
}

int compareNodesByRank(const void* firstNode, const void* secondNode)
{
    return ((infoNode*)firstNode)->rank - ((infoNode*)secondNode)->rank;
}

int compareNodesOfSameRankInOrder(const void* firstNode, const void* secondNode)
{
    return (((infoNode*)firstNode)->rank == ((infoNode*)secondNode)->rank)
                ? ((infoNode*)firstNode)->sequenceNumber - ((infoNode*)secondNode)->sequenceNumber
                : 0;
}

void goToSubtreeReading(tNode* node, char* dataArray, int64_t fileSize)
{
    assert(node);
    assert(dataArray);

    static int pos = 1;
    // while (pos < fileSize)
    // {
    //     switch (dataArray[pos])
    //     {
    //         case '(':
    //         {
    //             if (isdigit(dataArray[pos + 1]))
    //             node->left = memoryAllocationForNode();
    //             node->right = memoryAllocationForNode();
    //             goToSubtreeReading(node->left, dataArray, fileSize);
    //             goToSubtreeReading(node->right, dataArray, fileSize);
//             }
//             break;
//             case ')':
//             {
//                 return;
//             }
//             break;
//             case isOperation(dataArray[pos]):
//             {
//
//             }
//             break;
//             default: assert(0);
//         }
//     }
}


// void treeCalculating(tNode* node)
// {
//     assert(node);
//
//     bool calculationIsPossible = false;
//     if (node->left->type == Number && node->right->type == Number)
//     {
//
//     }
// }



// void readingExpressionFromFile(tNode* root)
// {
//     FILE* expressionFile = fopen("expression.txt", "r"); // FIXME const, assert
//
//     fseek(expressionFile, 0, SEEK_END);
//     int64_t fileSize = ftello(expressionFile);
//     fseek(expressionFile, 0, SEEK_SET);
//
//     char* dataArray = (char*)calloc((size_t)fileSize + 1, sizeof(char));
//     assert(dataArray);
//     fread(dataArray, sizeof(char), (size_t)fileSize, expressionFile);
//
//     for (int i = 0; i < fileSize; i++) printf("%c", dataArray[i]); // NOTE DEBUG
//
//     char* ptr = nullptr;
//     infoNode* dataArrayClear = (infoNode*)calloc(1, sizeof(infoNode));
//     int index = 0;
//
//     int rank = 0;
//     bool firstUseOfStrtokFunction = true;
//
//     printf("\n"); // NOTE DEBUG
//
//     for (int i = 0; i < fileSize; i++)
//     {
//         if (dataArray[i] == '\0') { rank++; continue; }
//         if      (dataArray[i] == '(') { rank++;  }
//         else if (dataArray[i] == ')') { rank--;  }
//         else
//         {  printf("_%c_", dataArray[i]);
//             (firstUseOfStrtokFunction) ? ptr = strtok(dataArray, "(")
//                                        : ptr = strtok(NULL     , "(");
//             firstUseOfStrtokFunction = false;
//             // dataArray[i - 1] = '(';
//
//             dataArrayClear = (infoNode*)realloc(dataArrayClear, (index + 1) * sizeof(infoNode));
//             assert(dataArrayClear);
//
//             dataArrayClear[index].data = ptr;
//             dataArrayClear[index].rank = rank;
//
//
//
//             index++;
//             while (dataArray[i] != '(' && dataArray[i] != ')' && i < fileSize && dataArray[i] != '\0') i++;
//             i--;
//         }
//     }
// printf("\n"); // NOTE DEBUG
//     // for (int j = 0; j < index; j++) printf("_%s_%d_", dataArrayClear[j].data, dataArrayClear[j].rank); // NOTE DEBUG
//     for (int j = 0; j < index; j++) printf("_%d_", dataArrayClear[j].rank); // NOTE DEBUG
//
//     goToSubtreeReading(root, dataArray, fileSize);
//
//     FREE(dataArrayClear);
//     FREE(dataArray);
//     FCLOSE(expressionFile);
// }
