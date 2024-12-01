#include "dsl.h"
#include "parser.h"
#include "node.h"
#include "tree.h"
#include "differentiator.h"

#define $ fprintf(stderr, "%s:%d in function: %s\n", __FILE__, __LINE__, __func__);

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
