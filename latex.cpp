#include "latex.h"

#include <assert.h>

#include "differentiator.h"
#include "node.h"
#include "debug.h"

// static --------------------------------------------------------------------------------------------------------------

static void printSimplifications(tNode* root, FILE* file, const char* const nameOfMathFunction);
static void dumpLatexTreeTraversal(tNode* node, FILE* latexFile);

// global --------------------------------------------------------------------------------------------------------------

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
            "\\section*{%s}\n", kDocumentTitle);

    fprintf(latexFile, kPhraseAboutOriginalFunction);
    printSimplifications(root, latexFile, kNameOfOriginalFunction);
    dump(root, kDumpFileName);

    tNode* firstDerivative = diff(root);
    fprintf(latexFile, kPhraseAboutFirstDerivative);
    printSimplifications(firstDerivative, latexFile, kNameOfFirstDerivativeFunction);
    dump(firstDerivative, kFirstDerivativeDumpFileName);

    tNode* secondDerivative = diff(firstDerivative);
    fprintf(latexFile, kPhraseAboutSecondDerivative);
    printSimplifications(secondDerivative, latexFile, kNameOfSecondDerivativeFunction);
    dump(secondDerivative, kSecondDerivativeDumpFileName);

    treeDtor(firstDerivative);
    treeDtor(secondDerivative);

    fprintf(latexFile,
        "%s\n"
        "\\includegraphics[width=1\\linewidth]{%s}\n"
        "\\end{center}\n"
        "\n\\end{document}\n", kPhraseAtTheEnd, kMemePictureName);

    FCLOSE(latexFile);
}

// static --------------------------------------------------------------------------------------------------------------

static void printSimplifications(tNode* root, FILE* file, const char* const nameOfMathFunction)
{
    int numberOfSimplifications = 0;
    bool expressionIsBeingConverted = true;

    while (expressionIsBeingConverted)
    {
        fprintf(file, "\\[%s = ", nameOfMathFunction);
        dumpLatexTreeTraversal(root, file);
        fprintf(file, "\\]\n");

        simplificationByCalc(root, &numberOfSimplifications);
        expressionIsBeingConverted = (numberOfSimplifications) ? true : false;
        numberOfSimplifications = 0;
    }
}

static void dumpLatexTreeTraversal(tNode* node, FILE* latexFile)
{
    assert(node);
    assert(latexFile);

    if (node->type == Number)
    {
        fprintf(latexFile, "%d", node->value);
    }
    else if (node->type == Variable)
    {
        fprintf(latexFile, "x");
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
                fprintf(latexFile, " (-");
                dumpLatexTreeTraversal(node->left, latexFile);
                fprintf(latexFile, ") ");
            }
            break;
            default: assert(0);
        }
    }
    else assert(0);
}
