#ifndef DSL_H
#define DSL_H

#define _ADD(leftNode, rightNode) \
    newNode(Operation, Add, leftNode, rightNode)
#define _SUB(leftNode, rightNode) \
    newNode(Operation, Sub, leftNode, rightNode)
#define _MUL(leftNode, rightNode) \
    newNode(Operation, Mul, leftNode, rightNode)
#define _DIV(leftNode, rightNode) \
    newNode(Operation, Div, leftNode, rightNode)

#endif // DSL_H
