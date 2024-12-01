#include "differentiator.h"

#include "tree.h"
#include "dsl.h"
#include "debug.h"

#include <assert.h>

tNode* diff(tNode* node)
{
    if (node->type == Number   ) return _NUM(0);
    if (node->type == Variable ) return _NUM(1);
    if (node->type == Operation)
    {
        switch (node->value)
        {
            case Add:
            {
                return
                _ADD(diff(node->left), diff(node->right));
            }
            break;
            case Sub:
            {
                return
                _SUB(diff(node->left), diff(node->right));
            }
            break;
            case Mul:
            {
                return
                _ADD(
                    _MUL(diff(node->left ), copyNode(node->right)),
                    _MUL(diff(node->right), copyNode(node->left))
                );
            }
            break;
            case Div:
            {
                return
                _DIV(
                    _SUB(
                        _MUL(diff(node->left ), copyNode(node->right)),
                        _MUL(diff(node->right), copyNode(node->left))
                    ),
                    _MUL(copyNode(node->right), copyNode(node->right))
                );
            }
            break;
            case Deg:
            {
                return diffDegree(node);
            }
            break;
            case Ln:
            {
                return diffLn(node);
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
        return _NUM(0);
    }
    else if (!subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right))
    {
        // TODO , need diffLn
    }

    else if (subtreeContainsVariable(node->left) && !subtreeContainsVariable(node->right))
    {
        return
        _MUL(
            diff(node->left),
            _MUL(
                copyNode(node->right),
                _DEG(
                    copyNode(node->left),
                    _SUB(copyNode(node->right), _NUM(1))
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
        _MUL(
            diff(node->left),
            _DIV(
                _NUM(1),
                copyNode(node->left)
            )
        );
    }
    else
    {}
}
