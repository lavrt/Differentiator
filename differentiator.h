#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "node.h"

tNode* diff(tNode* root);
int simplificationByCalc(tNode* node);

tNode* diffAdd(tNode* node);
tNode* diffSub(tNode* node);
tNode* diffMul(tNode* node);
tNode* diffDiv(tNode* node);
tNode* diffDeg(tNode* node);
tNode* diffLn (tNode* node);
tNode* diffLog(tNode* node);
tNode* diffLg (tNode* node);
tNode* diffSin(tNode* node);
tNode* diffCos(tNode* node);
tNode* diffTg (tNode* node);
tNode* diffCtg(tNode* node);

#endif // DIFFERENTIATOR_H
