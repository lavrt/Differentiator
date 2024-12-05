#ifndef DSL_H
#define DSL_H

#define NUM(value_) \
    newNode(Number, value_, NULL, NULL)
#define VAR(value_) \
    newNode(Variable, value_, NULL, NULL)
#define ADD(leftNode_, rightNode_) \
    newNode(Operation, Add, leftNode_, rightNode_)
#define SUB(leftNode_, rightNode_) \
    newNode(Operation, Sub, leftNode_, rightNode_)
#define MUL(leftNode_, rightNode_) \
    newNode(Operation, Mul, leftNode_, rightNode_)
#define DIV(leftNode_, rightNode_) \
    newNode(Operation, Div, leftNode_, rightNode_)
#define DEG(leftNode_, rightNode_) \
    newNode(Operation, Deg, leftNode_, rightNode_)

#endif // DSL_H
