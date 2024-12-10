#ifndef LATEX_H
#define LATEX_H

#include <stdio.h>

#include "tree.h"

const char* const kLatexFileName = "derivative.tex";

void latexGeneration(tNode* root);
void dumpLatexTreeTraversal(tNode* node, FILE* latexFile);
// void preOrder(tNode* node, FILE* latexFile);
// void inOrder(tNode* root, FILE* latexFile);
// void postOrder(tNode* root, FILE* latexFile);

#endif // LATEX_H
