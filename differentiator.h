#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "node.h"

tNode* diff(tNode* node);
tNode* diffAdd(tNode* node);
tNode* diffSub(tNode* node);
tNode* diffMul(tNode* node);
tNode* diffDiv(tNode* node);
tNode* diffDeg(tNode* node);
tNode* diffLn(tNode* node);

#endif // DIFFERENTIATOR_H
