/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() = default;

Statement::~Statement() = default;

Comment::Comment(TokenScanner &scanner) {}

Comment::~Comment() = default;

int Comment::execute(EvalState & state) {return 0;}

End::End(TokenScanner &scanner) {}

End::~End() = default;

int End::execute(EvalState &state) {return -1;}

Assignment::Assignment(TokenScanner &scanner) {
    try {
        exp = parseExp(scanner);
    }
    catch(ErrorException &ex) {
        error("SYNTAX ERROR");

    }
}

Assignment::~Assignment() {
    delete exp;
}

int Assignment::execute(EvalState & state) {
    try {
        exp->eval(state);
    }
    catch(ErrorException &ex) {
        throw ex;
    }
    return 0;
}

Print::Print(TokenScanner &scanner) {
    try {
        exp = parseExp(scanner);
    }
    catch(ErrorException &ex) {
        throw ex;
        //error("SYNTAX ERROR");
    }
}

Print::~Print() {
    delete exp;
}

int Print::execute(EvalState &state) {
    try {
        cout << exp->eval(state) << endl;
    }
    catch(ErrorException &ex) {
        throw ex;
    }
    return 0;
}

Input::Input(TokenScanner &scanner) {
    var = scanner.nextToken();
    if (scanner.getTokenType(var) != WORD || scanner.hasMoreTokens())
        error("SYNTAX ERROR");
}

Input::~Input() = default;

int Input::execute(EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    string str;
    string token;
    while(true) {
        cout << " ? ";
        getline(cin, str);
        scanner.setInput(str);
        token = scanner.nextToken();
        try {
            if(scanner.hasMoreTokens()) {
                if(token == "-") {
                    token = scanner.nextToken();
                    if(scanner.hasMoreTokens())
                        error("SYNTAX ERROR");
                    state.setValue(var, -stringToInteger(token));
                    break;
                }
                error("SYNTAX ERROR");
            } else {
                state.setValue(var, stringToInteger(token));
                break;
            }
        }
        catch(ErrorException &ex) {
            cout << "INVALID NUMBER" << endl;
        }
    }
    // TODO: check again
    return 0;
}

Transfer::Transfer(TokenScanner &scanner) {
    string token = scanner.nextToken();
    try {
        lineNum = stringToInteger(token);
    }
    catch(ErrorException &ex) {
        throw ex;
        //error("SYNTAX ERROR");
    }
}

Transfer::~Transfer() = default;

int Transfer::execute(EvalState &state) {return lineNum;}

Condition::Condition(TokenScanner &scanner) {
    try {
        lhs = readE(scanner, precedence("="));
        op = scanner.nextToken();
        rhs = readE(scanner, precedence("="));
        if(scanner.getTokenType(op) != OPERATOR)
            error("SYNTAX ERROR");
        string token = scanner.nextToken();    //THEN
        token = scanner.nextToken();
        lineNum = stringToInteger(token);
    }
    catch(ErrorException &ex) {
        throw ex;
        //error("SYNTAX ERROR");
    }
}

Condition::~Condition() = default;

int Condition::execute(EvalState &state) {
    try {
        int left = lhs->eval(state);
        int right = rhs->eval(state);
        if(op == ">")
            return left > right ? lineNum : 0;
        else if(op == "<")
            return left < right ? lineNum : 0;
        else if(op == "=")
            return left == right ? lineNum : 0;
        else  error("SYNTAX ERROR");
    }
    catch(ErrorException &ex) {
        throw ex;
    }
}
