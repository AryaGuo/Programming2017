/*
 * File: Basic.cpp
 * ---------------
 * Name: Arya
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(const string &line, Program & program, EvalState & state);

/* Main program */

int main()
{
    EvalState state;
    Program program;
    string line;
    while (getline(cin, line)) {
        try {
            processLine(line, program, state);
        } catch (ErrorException & ex) {
            //cerr << "Error: " << ex.getMessage() << endl;
            cout << ex.getMessage() << endl;
        }
    }
    return 0;
}

void showHelp()
{
    cout << "This is a BASIC interpreter" << endl;
}

void processCode(const int &lineNum, const string &line, Program &program, TokenScanner &scanner)
{
    if(scanner.hasMoreTokens()) {
        program.addSourceLine(lineNum, line);
        string token = scanner.nextToken();
        Statement *st = nullptr;
        if(token == "REM")  st = new Comment(scanner);
        else if(token ==  "LET")    st = new Assignment(scanner);
        else if(token ==  "PRINT")  st = new Print(scanner);
        else if(token ==  "INPUT")  st = new Input(scanner);
        else if(token ==  "END")    st = new End(scanner);
        else if(token ==  "GOTO")    st = new Transfer(scanner);
        else if(token ==  "IF") st = new Condition(scanner);
        else    error("SYNTAX ERROR");
        program.setParsedStatement(lineNum, st);
    }
    else {
        program.removeSourceLine(lineNum);
    }
}

void processCom(const string &token, const string &line, Program &program, EvalState &state, TokenScanner &scanner)
{
    if(token == "RUN") {
        if(scanner.hasMoreTokens())
            error("SYNTAX ERROR");
        program.run(state);
    } else if(token == "LIST") {
        if(scanner.hasMoreTokens())
            error("SYNTAX ERROR");
        program.list();
    } else if(token == "CLEAR") {
        if(scanner.hasMoreTokens())
            error("SYNTAX ERROR");
        program.clear();
        state.clear();
    } else if(token == "HELP") {
        if(scanner.hasMoreTokens())
            error("SYNTAX ERROR");
        showHelp();
    } else if(token == "QUIT") {
        if(scanner.hasMoreTokens())
            error("SYNTAX ERROR");
        //cout << "The program is ended." << endl;
        exit(0);
    } else if(token == "LET") {
        try {
            Statement *st = new Assignment(scanner);
            st->execute(state);
        }
        catch(ErrorException &ex) {
            throw ex;
        }
    } else if(token == "INPUT") {
        try {
            Statement *st = new Input(scanner);
            st->execute(state);
        }
        catch(ErrorException &ex) {
            throw ex;
        }
    } else if(token == "PRINT") {
        try {
            Statement *st = new Print(scanner);
            st->execute(state);
        }
        catch(ErrorException &ex) {
            throw ex;
        }
    } else {
        error("SYNTAX ERROR");
    }
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(const string &line, Program & program, EvalState & state)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
//	Expression *exp = parseExp(scanner);
//	int value = exp->eval(state);
//	cout << value << endl;
//	delete exp;
    string token = scanner.nextToken();
    if(scanner.getTokenType(token) == NUMBER) {
        processCode(stringToInteger(token), line, program, scanner);
    } else if(scanner.getTokenType(token) == WORD) {
        processCom(token, line, program, state, scanner);
    } else {
        error("SYNTAX ERROR");
    }
}
