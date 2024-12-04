#ifndef NODE_H
#define NODE_H

enum NodeType
{
    Number    = 1,
    Variable  = 2,
    Operation = 3,
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
    Log = 7,
    Sin = 8,
    Cos = 9,
    Tg  = 10,
    Ctg = 11,
};

#endif // NODE_H
