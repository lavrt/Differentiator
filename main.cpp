#include "dsl.h"
#include "parser.h"
#include "node.h"
#include "tree.h"
#include "differentiator.h"

int main()
{
    tNode* root = getG();

    dump(root, kDumpFileName);

    tNode* diffRoot = diff(root);

    dump(diffRoot, kDiffDumpFileName);

    treeDtor(diffRoot);

    treeDtor(root);

    return 0;
}
