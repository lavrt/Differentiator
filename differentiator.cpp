#include "differentiator.h"

#include "tree.h"
#include "dsl.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>

// static --------------------------------------------------------------------------------------------------------------

static tNode* diffAdd(tNode* node);
static tNode* diffSub(tNode* node);
static tNode* diffMul(tNode* node);
static tNode* diffDiv(tNode* node);
static tNode* diffDeg(tNode* node);
static tNode* diffLn (tNode* node);
static tNode* diffLog(tNode* node);
static tNode* diffLg (tNode* node);
static tNode* diffSin(tNode* node);
static tNode* diffCos(tNode* node);
static tNode* diffTg (tNode* node);
static tNode* diffCtg(tNode* node);

// global --------------------------------------------------------------------------------------------------------------

void simplificationByCalc(tNode* node, int* counter)
{
    assert(node);

    assert(node->type == Operation);

    if (node->left  && node->right
        && node->left->type == Number && node->right->type == Number)
    {
        (*counter)++;

        switch (node->value)
        {
            case Add:
            {
                node->type = Number;
                node->value = node->left->value + node->right->value;
                FREE(node->left); FREE(node->right);
            }
            break;
            case Sub:
            {
                node->type = Number;
                node->value = node->left->value - node->right->value;
                FREE(node->left); FREE(node->right);
            }
            break;
            case Mul:
            {
                node->type = Number;
                node->value = node->left->value * node->right->value;
                FREE(node->left); FREE(node->right);
            }
            break;
            default:;
        }
    }

    if (node->type == Operation && node->value == Add
        && ((node->left->type == Number && node->left->value == 0)
        || (node->right->type == Number && node->right->value == 0)))
    {
        (*counter)++;

        if (node->left->value == 0)
        {
            FREE(node->left);

            tNode* oldNode = node->right;

            node->type = node->right->type;
            node->value = node->right->value;
            node->left = node->right->left;
            node->right = node->right->right;

            FREE(oldNode);
        }
        else
        {
            FREE(node->right);

            tNode* oldNode = node->left;

            node->type = node->left->type;
            node->value = node->left->value;
            node->right = node->left->right;
            node->left = node->left->left;

            FREE(oldNode);
        }
    }

    if (node->type == Operation && node->value == Sub
        && ((node->right->type == Number && node->right->value == 0)
        || (node->left->type == Number && node->left->value == 0)))
    {
        (*counter)++;

        if (node->right->value == 0)
        {
            FREE(node->right);

            tNode* oldNode = node->left;

            node->type = node->left->type;
            node->value = node->left->value;
            node->right = node->left->right;
            node->left = node->left->left;

            FREE(oldNode);
        }
        else
        {
            FREE(node->left);
            node->left = node->right;
            node->right = NULL;
            node->value = uSub;
        }
    }

    if (node->type == Operation && node->value == Mul
        && ((node->left->type == Number && node->left->value == 0)
        || (node->right->type == Number && node->right->value == 0)))
    {
        (*counter)++;

        node->type = Number;
        node->value = 0;
        treeDtor(node->left);
        treeDtor(node->right);
        node->left = NULL;
        node->right = NULL;
    }

    if (node->type == Operation && node->value == Mul
        && ((node->left->type == Number && node->left->value == 1)
        || (node->right->type == Number && node->right->value == 1)))
    {
        (*counter)++;

        if (node->left->value == 1)
        {
            FREE(node->left);

            tNode* oldNode = node->right;

            node->type = node->right->type;
            node->value = node->right->value;
            node->left = node->right->left;
            node->right = node->right->right;

            FREE(oldNode);
        }
        else
        {
            FREE(node->right);

            tNode* oldNode = node->left;

            node->type = node->left->type;
            node->value = node->left->value;
            node->right = node->left->right;
            node->left = node->left->left;

            FREE(oldNode);
        }
    }

    if (node->type == Operation && node->value == Div
        && node->right->type == Number && node->right->value == 1)
    {
        (*counter)++;

        FREE(node->right);

        tNode* oldNode = node->left;

        node->type = node->left->type;
        node->value = node->left->value;
        node->right = node->left->right;
        node->left = node->left->left;

        FREE(oldNode);
    }

    if (node->type == Operation && node->value == Deg
        && ((node->right->type == Number && node->right->value == 0)
        || (node->right->type == Number && node->right->value == 1)))
    {
        (*counter)++;

        if (node->right->value == 0)
        {
            FREE(node->right);
            treeDtor(node->left);
            node->left = NULL;
            node->type = Number;
            node->value = 1;
        }
        else
        {
            FREE(node->right);

            tNode* oldNode = node->left;

            node->type = node->left->type;
            node->value = node->left->value;
            node->right = node->left->right;
            node->left = node->left->left;

            FREE(oldNode);
        }
    }

    if (node->left && (node->left->type == Operation)) simplificationByCalc(node->left, counter);
    if (node->right && (node->right->type == Operation)) simplificationByCalc(node->right, counter);
}

tNode* diff(tNode* node)
{
    assert(node);

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
            case Log: return diffLog(node); break;
            case Lg : return diffLg (node); break;
            case Sin: return diffSin(node); break;
            case Cos: return diffCos(node); break;
            case Tg : return diffTg (node); break;
            case Ctg: return diffCtg(node); break;

            default: assert(0);
        }
    }

    else assert(0);
}

// static --------------------------------------------------------------------------------------------------------------

static tNode* diffAdd(tNode* node)
{
    assert(node);

    return
        ADD(diff(node->left), diff(node->right));
}

static tNode* diffSub(tNode* node)
{
    assert(node);

    return
        SUB(diff(node->left), diff(node->right));
}

static tNode* diffMul(tNode* node)
{
    assert(node);

    return
        ADD(
            MUL(diff(node->left ), copyNode(node->right)),
            MUL(diff(node->right), copyNode(node->left))
        );
}

static tNode* diffDiv(tNode* node)
{
    assert(node);

    return
        DIV(
            SUB(
                MUL(diff(node->left ), copyNode(node->right)),
                MUL(diff(node->right), copyNode(node->left))
            ),
            MUL(copyNode(node->right), copyNode(node->right))
        );
}

static tNode* diffDeg(tNode* node)
{
    assert(node);

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
    else if (subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right))
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

static tNode* diffLn(tNode* node)
{
    assert(node);

    if (subtreeContainsVariable(node->left))
    {
        return
            DIV(
                diff(node->left),
                copyNode(node->left)
        );
    }
    else
    {
        return NUM(0);
    }
}

static tNode* diffLog(tNode* node)
{
    assert(node);

    if (!subtreeContainsVariable(node->left) && !subtreeContainsVariable(node->right))
    {
        return NUM(0);
    }
    else if (!subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right))
    {
        return
            DIV(
                diff(node->right),
                MUL(
                    copyNode(node->right),
                    newNode(Operation, Ln, copyNode(node->left), NULL)
                )
            );
    }
    else if (subtreeContainsVariable(node->left) && !subtreeContainsVariable(node->right))
    {
        return
            DIV(
                SUB(
                    NUM(0),
                    diff(node->left)
                ),
                MUL(
                    copyNode(node->left),
                    MUL(
                        newNode(Operation, Ln, copyNode(node->right), NULL),
                        DEG(
                            newNode(Operation, Log, copyNode(node->right), copyNode(node->left)),
                            NUM(2)
                        )
                    )
                )
            );

    }
    else if (subtreeContainsVariable(node->left) && subtreeContainsVariable(node->right))
    {
        return
            DIV(
                SUB(
                    MUL(
                        diff(node->right),
                        MUL(
                            copyNode(node->left),
                            newNode(Operation, Ln, copyNode(node->left), NULL)
                        )
                    ),
                    MUL(
                        diff(node->left),
                        MUL(
                            copyNode(node->right),
                            newNode(Operation, Ln, copyNode(node->right), NULL)
                        )
                    )
                ),
                MUL(
                    DEG(
                        newNode(Operation, Ln, copyNode(node->left), NULL),
                        NUM(2)
                    ),
                    MUL(
                        copyNode(node->left),
                        copyNode(node->right)
                    )
                )
            );
    }
    else assert(0);
}

static tNode* diffLg(tNode* node)
{
    assert(node);

    return
        DIV(
            diff(node->left),
            MUL(
                copyNode(node->left),
                newNode(Operation, Ln, NUM(10), NULL)
            )
        );
}

static tNode* diffSin(tNode* node)
{
    assert(node);

    return
        MUL(
            diff(node->left),
            newNode(Operation, Cos, copyNode(node->left), NULL)
        );
}

static tNode* diffCos(tNode* node)
{
    assert(node);

    return
        MUL(
            diff(node->left),
            SUB(
                NUM(0),
                newNode(Operation, Sin, copyNode(node->left), NULL)
            )
        );
}

static tNode* diffTg(tNode* node)
{
    assert(node);

    return
        DIV(
            diff(node->left),
            DEG(
                newNode(Operation, Cos, copyNode(node->left), NULL),
                NUM(2)
            )
        );
}

static tNode* diffCtg(tNode* node)
{
    assert(node);

    return
        SUB(
            NUM(0),
            DIV(
                diff(node->left),
                DEG(
                    newNode(Operation, Sin, copyNode(node->left), NULL),
                    NUM(2)
                )
            )
        );
}
