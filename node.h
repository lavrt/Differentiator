#ifndef NODE_H
#define NODE_H

enum NodeType
{
    Number    = 1,
    Variable  = 2,
    Operation = 4,
};

struct tNode
{
    NodeType type;
    int value;
    tNode* left;
    tNode* right;
};

enum Operations
{
    NoOperation = 0,
    Add = 1,
    Sub = 2,
    Mul = 3,
    Div = 4,
    Deg = 5,
    Ln  = 6,
};

#endif // NODE_H
