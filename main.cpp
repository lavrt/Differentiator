#include "dsl.h"
#include "parser.h"
#include "node.h"
#include "tree.h"
#include "differentiator.h"
#include "latex.h"

int main()
{
    tNode* root = runParser();

    dump(root, kDumpFileName);

    tNode* diffRoot = diff(root);

    dump(diffRoot, kDiffDumpFileName);

    latexGeneration(diffRoot);

    treeDtor(diffRoot);

    treeDtor(root);

    return 0;
}
