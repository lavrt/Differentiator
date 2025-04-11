#include "dsl.h"
#include "parser.h"
#include "node.h"
#include "tree.h"
#include "differentiator.h"
#include "latex.h"

int main()
{
    tNode* root = runParser();

    latexGeneration(root);

    treeDtor(root);

    return 0;
}
