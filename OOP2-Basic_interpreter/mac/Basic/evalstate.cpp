/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"
using namespace std;

/* Implementation of the EvalState class */

static map <string, bool> reserve;
static bool flag = false;

EvalState::EvalState() {
    if(!flag) {
        flag = true;
        reserve["IF"] = 1;
        reserve["REM"] = 1;
        reserve["RUN"] = 1;
        reserve["LET"] = 1;
        reserve["END"] = 1;
        reserve["GOTO"] = 1;
        reserve["THEN"] = 1;
        reserve["LIST"] = 1;
        reserve["QUIT"] = 1;
        reserve["HELP"] = 1;
        reserve["INPUT"] = 1;
        reserve["PRINT"] = 1;
        reserve["CLEAR"] = 1;
    }
}

EvalState::~EvalState() = default;

void EvalState::setValue(string var, int value) {
    if(reserve[var])
        error("SYNTAX ERROR");
    symbolTable.put(var, value);
}

int EvalState::getValue(string var) {
    if(reserve[var])
        error("SYNTAX ERROR");
    return symbolTable.get(var);
}

bool EvalState::isDefined(string var) {
    if(reserve[var])
        error("SYNTAX ERROR");
    return symbolTable.containsKey(var);
}

void EvalState::clear() {
    symbolTable.clear();
}
