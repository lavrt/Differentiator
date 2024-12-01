#ifndef PARSER_H
#define PARSER_H

#include "node.h"

tNode* getG();
tNode* getE();
tNode* getT();
tNode* getD();
tNode* getP();
tNode* getN();
tNode* getV();
[[noreturn]] void syntaxError();

#endif // PARSER_H
