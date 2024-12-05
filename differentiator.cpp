#include "differentiator.h"

#include "tree.h"
#include "dsl.h"
#include "debug.h"

#include <assert.h>

tNode* diff(tNode* node)
{
    if (node->type == Number   ) return NUM(0);
    if (node->type == Variable ) return NUM(1);
    if (node->type == Operation)
    {
        switch (node->value)
        {
            case Add: return diffAdd(node); break;
            case Sub: return diffSub(node); break;
            case Mul: return diffMul(node); break;
            case Div: return diffDiv(node); break;
            case Deg: return diffDeg(node); break;
            case Ln : return diffLn (node); break;

            default: assert(0);
        }
    }

    else assert(0);
}

tNode* diffAdd(tNode* node)
{
    return
        ADD(diff(node->left), diff(node->right));
}

tNode* diffSub(tNode* node)
{
    return
        SUB(diff(node->left), diff(node->right));
}

tNode* diffMul(tNode* node)
{
    return
        ADD(
            MUL(diff(node->left ), copyNode(node->right)),
            MUL(diff(node->right), copyNode(node->left))
        );
}

tNode* diffDiv(tNode* node)
{
    return
        DIV(
            SUB(
                MUL(diff(node->left ), copyNode(node->right)),
                MUL(diff(node->right), copyNode(node->left))
            ),
            MUL(copyNode(node->right), copyNode(node->right))
        );
}

tNode* diffDeg(tNode* node)
{
    if (!subtreeContainsVariable(node->left) && !subtreeContainsVariable(node->right))
    {
        return NUM(0);
    }
    else if (!subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right))
    {
        return
            MUL(
                diff(node->right),
                MUL(
                    copyNode(node),
                    newNode(Operation, Ln, copyNode(node->left), NULL)
                )
            );
    }
    else if (subtreeContainsVariable(node->left) && !subtreeContainsVariable(node->right))
    {
        return
            MUL(
                diff(node->left),
                MUL(
                    copyNode(node->right),
                    DEG(
                        copyNode(node->left),
                        SUB(copyNode(node->right), NUM(1))
                    )
                )
            );
    }
    else if (subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right)) // FIXME
    {
        return
            MUL(
                copyNode(node),
                ADD(
                    MUL(
                        diff(node->right),
                        newNode(Operation, Ln, copyNode(node->left), NULL)
                    ),
                    MUL(
                        diff(node->left),
                        copyNode(node->right)
                    )
                )
            );
    }
    else assert(0);
}

tNode* diffLn(tNode* node)
{
    if (subtreeContainsVariable(node->left))
    {
        return
            MUL(
                diff(node->left),
                DIV(
                    NUM(1),
                    copyNode(node->left)
                )
            );
    }
    else
    {
        return NUM(0);
    }
}
