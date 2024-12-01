#ifndef DSL_H
#define DSL_H

#define _NUM(value_) \
    newNode(Number, value_, NULL, NULL)
#define _VAR(value_) \
    newNode(Variable, value_, NULL, NULL)
#define _ADD(leftNode_, rightNode_) \
    newNode(Operation, Add, leftNode_, rightNode_)
#define _SUB(leftNode_, rightNode_) \
    newNode(Operation, Sub, leftNode_, rightNode_)
#define _MUL(leftNode_, rightNode_) \
    newNode(Operation, Mul, leftNode_, rightNode_)
#define _DIV(leftNode_, rightNode_) \
    newNode(Operation, Div, leftNode_, rightNode_)
#define _DEG(leftNode_, rightNode_) \
    newNode(Operation, Deg, leftNode_, rightNode_)

#endif // DSL_H
