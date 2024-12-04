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
            case Add:
            {
                return
                    ADD(diff(node->left), diff(node->right));
            }
            break;
            case Sub:
            {
                return
                    SUB(diff(node->left), diff(node->right));
            }
            break;
            case Mul:
            {
                return
                    ADD(
                        MUL(diff(node->left ), copyNode(node->right)),
                        MUL(diff(node->right), copyNode(node->left))
                    );
            }
            break;
            case Div:
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
            break;
            case Deg:
            {
                return
                    diffDegree(node);
            }
            break;
            case Ln:
            {
                return
                    diffLn(node);
            }
            break;
            default: assert(0);
        }
    }
}

tNode* diffDegree(tNode* node)
{
    if (!subtreeContainsVariable(node->left) && !subtreeContainsVariable(node->right))
    {
        return NUM(0);
    }
    else if (!subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right))
    {
        // TODO , need diffLn
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
        // TODO
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
        return NUM(0); // NOTE Check
    }
}
