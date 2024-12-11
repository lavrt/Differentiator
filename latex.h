#ifndef LATEX_H
#define LATEX_H

#include <stdio.h>

#include "tree.h"

const char* const kLatexFileName = "derivative.tex";
const char* const kMemePictureName = "mem.jpg";

const char* const kNameOfOriginalFunction = "f";
const char* const kNameOfFirstDerivativeFunction = "f^\\prime";
const char* const kNameOfSecondDerivativeFunction = "f^{\\prime\\prime}";

const char* const kDocumentTitle =
    "Differentiation of a mathematical expression";
const char* const kPhraseAboutOriginalFunction =
    "The original function, of which very soon there will be no trace:";
const char* const kPhraseAboutFirstDerivative =
    "The first derivative is calculated in the easiest way as follows:";
const char* const kPhraseAboutSecondDerivative =
    "Further, the calculation of the second derivative is obvious:";
const char* const kPhraseAtTheEnd =
    "But in fact, children calculate this in kindergarten, the expression could be given more complicated)";

void latexGeneration(tNode* root);

#endif // LATEX_H
