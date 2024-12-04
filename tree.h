#ifndef TREE_H
#define TREE_H

#include "node.h"

#include <stdio.h>
#include <stdbool.h>

#define FREE(ptr_) \
    do { free(ptr_); ptr_ = nullptr; } while(0);
#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = nullptr; } while(0);

const char* const kDumpFileName = "dump.gv";
const char* const kDiffDumpFileName = "diffDump.gv";

const char* const kNumber = "number";
const char* const kVariable = "variable";
const char* const kOperation = "operation";

const char* const kAdd = "+";
const char* const kSub = "-";
const char* const kMul = "*";
const char* const kDiv = "/";
const char* const kDeg = "^";
const char* const kLn  = "ln";
const char* const kLog = "log";
const char* const kSin = "sin";
const char* const kCos = "cos";
const char* const kTg  = "tg";
const char* const kCtg = "ctg";

tNode* newNode(NodeType type, int value, tNode* left, tNode* right);
tNode* memoryAllocationForNode(void);
void treeDtor(tNode* node);
void dump(tNode* root, const char* const dumpFileName);
void dumpTreeTraversal(tNode* node, FILE* dumpFile);
void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);
void printOperationType(tNode* node, FILE* dumpFile);
tNode* copyNode(tNode* node);
bool subtreeContainsVariable(tNode* node);

#endif // TREE_H
