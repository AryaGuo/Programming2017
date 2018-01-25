/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include <utility>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() = default;

Program::~Program() {
    mp.clear();
}

void Program::clear() {
    Program::~Program();
}

void Program::addSourceLine(int lineNumber, string line) {
    mp[lineNumber] = node(lineNumber, std::move(line));
}

void Program::removeSourceLine(int lineNumber) {
    mp.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    return mp[lineNumber].source_line;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if(mp.count(lineNumber))
        mp[lineNumber].parsed_sta = stmt;
    else
        error("LINE NUMBER ERROR");
}

Statement *Program::getParsedStatement(int lineNumber) {
    if(mp.count(lineNumber))
        return mp[lineNumber].parsed_sta;
    return nullptr;
}

int Program::getFirstLineNumber() {
    if(mp.begin() == mp.end())
        return -1;
    return mp.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = mp.upper_bound(lineNumber);
    if(it == mp.end())
        return -1;
    return it->first;
}

bool Program::existingLine(int lineNum) {
    return mp.count(lineNum);
}

void Program::list()
{
    int st = Program::getFirstLineNumber();
    while(st > 0)
    {
        cout << getSourceLine(st) << endl;
        st = getNextLineNumber(st);
    }
}

void Program::run(EvalState &state)
{
    try {
        int num = getFirstLineNumber();
        while(num > 0) {
            if(!existingLine(num))
                error("LINE NUMBER ERROR");
            Statement *st = getParsedStatement(num);
            int nxt = st->execute(state);
            if(nxt == 0)
                nxt = getNextLineNumber(num);
            num = nxt;
        }
    }
    catch(ErrorException &ex) {
        throw ex;
    }
}
