#include "latex.h"

#include <assert.h>

#include "differentiator.h"
#include "node.h"
#include "debug.h"

void latexGeneration(tNode* root)
{
    assert(root);

    FILE* latexFile = fopen(kLatexFileName, "w");
    assert(latexFile);

    fprintf(latexFile,
            "\\documentclass{article}\n"
            "\\usepackage{mathtools}\n"
            "\\usepackage{graphicx}\n"
            "\\usepackage{geometry}\n"
            "\\geometry{left=2cm,right=2cm,top=2cm,bottom=3cm,bindingoffset=0cm}\n\n"
            "\\begin{document}\n\n"
            "\\begin{center}\n"
            "\\section*{Differentiation of a mathematical expression}\n"
            "\\end{center}\n\n");

    int numberOfSimplifications = 0;

    bool expressionIsBeingConverted = true;
    while (expressionIsBeingConverted)
    {
        fprintf(latexFile, "\\[f^\\prime =");
        dumpLatexTreeTraversal(root, latexFile);
        fprintf(latexFile, "\\]\n");

        simplificationByCalc(root, &numberOfSimplifications);
        expressionIsBeingConverted = (numberOfSimplifications) ? true : false;
        numberOfSimplifications = 0;
    }

    fprintf(latexFile, "\n\\end{document}\n");

    FCLOSE(latexFile);
}

void dumpLatexTreeTraversal(tNode* node, FILE* latexFile)
{
    assert(node);
    assert(latexFile);

    if (node->type == Number)
    {
        fprintf(latexFile, "%d", node->value);
    }
    else if (node->type == Variable)
    {
        fprintf(latexFile, "x"); // FIXME
    }
    else if (node->type == Operation)
    {
        switch (node->value)
        {
            case Add:
            {
                fprintf(latexFile, "(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, " + ");
                dumpLatexTreeTraversal(node->right, latexFile);
                fprintf(latexFile, ")");
            }
            break;
            case Sub:
            {
                fprintf(latexFile, "(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, " - ");
                dumpLatexTreeTraversal(node->right, latexFile);
                fprintf(latexFile, ")");
            }
            break;
            case Mul:
            {
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, " \\cdot ");
                dumpLatexTreeTraversal(node->right, latexFile);
            }
            break;
            case Div:
            {
                fprintf(latexFile, " \\frac{");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, "}{");
                dumpLatexTreeTraversal(node->right, latexFile);
                fprintf(latexFile, "} ");
            }
            break;
            case Deg:
            {
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, "^{");
                dumpLatexTreeTraversal(node->right, latexFile);
                fprintf(latexFile, "} ");
            }
            break;
            case Ln:
            {
                fprintf(latexFile, " ln(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case Log:
            {
                fprintf(latexFile, " log_{");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, "}(");
                dumpLatexTreeTraversal(node->right, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case Lg:
            {
                fprintf(latexFile, " lg(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case Sin:
            {
                fprintf(latexFile, " sin(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case Cos:
            {
                fprintf(latexFile, " cos(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case Tg:
            {
                fprintf(latexFile, " tg(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case Ctg:
            {
                fprintf(latexFile, " ctg(");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            case uSub:
            {
                fprintf(latexFile, " -");
                dumpLatexTreeTraversal(node->left, latexFile);
            }
            break;
            default: assert(0);
        }
    }
    else assert(0);
}

// void preOrder(tNode* node, FILE* latexFile)
// {
//     if (node == NULL) return;
//     fprintf(latexFile, "%d ", node->value);
//     preOrder(node->left, latexFile);
//     preOrder(node->right, latexFile);
// }
//
// void inOrder(tNode* root, FILE* latexFile)
// {
//     if (root == NULL) return;
//     inOrder(root->left, latexFile);
//     fprintf(latexFile, "%d ", root->value);
//     inOrder(root->right, latexFile);
// }
//
// void postOrder(tNode* root, FILE* latexFile)
// {
//     if (root == NULL) return;
//     postOrder(root->left, latexFile);
//     postOrder(root->right, latexFile);
//     fprintf(latexFile, "%d ", root->value);
// }
