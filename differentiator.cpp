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
                return _ADD(diff(node->left), diff(node->right));
            }
            break;
            case Sub:
            {
                return _SUB(diff(node->left), diff(node->right));
            }
            break;
            case Mul:
            {
                return _ADD(
                            _MUL(diff(node->left ), copyNode(node->right)),
                            _MUL(diff(node->right), copyNode(node->left)));
            }
            break;
            case Div:
            {
                return _DIV(
                            _SUB(
                                 _MUL(diff(node->left ), copyNode(node->right)),
                                 _MUL(diff(node->right), copyNode(node->left))),
                            _MUL(copyNode(node->right), copyNode(node->right)));
            }
            break;
            case Deg:
            {
                return diffDegree(node);
            }
            break;
            default: assert(0);
        }
    }
}

tNode* diffDegree(tNode* node)
{
    if (node->left->type == Variable && node->right->type == Number)
    {
        return _MUL(
                    _NUM(node->right->value),
                    _DEG(
                         _VAR(node->left->value),
                         _NUM(node->right->value - 1)));
    }
    else if (node->left->type == Number && node->right->type == Variable)
    {
        // TODO
    }
    else if (node->left->type == Number && node->right->type == Number)
    {
        return _NUM(0);
    }
    else if (node->left->type == Variable && node->right->type == Variable)
    {
        // TODO
    }
    else assert(0);
}
