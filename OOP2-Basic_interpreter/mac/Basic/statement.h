/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/error.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */
// The parsed representation of that statement

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

    /*
     * Destructor: ~Statement
     * Usage: delete stmt;
     * -------------------
     * The destructor deallocates the storage for this expression.
     * It must be declared virtual to ensure that the correct subclass
     * destructor is called when deleting a statement.
     */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual int execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

/*
 * Class: Comment
 * --------------
 * This subclass represents a statement for comments.
 */

class Comment: public Statement {

public:
    explicit Comment(TokenScanner &scanner);
    ~Comment() override;
    int execute(EvalState & state) override;

};

/*
 * Class: Assignment
 * -----------------
 * This subclass represents a BASIC's assignment statement.
 */

class Assignment: public Statement {

public:
    explicit Assignment(TokenScanner &scanner) ;
    ~Assignment() override;
    int execute(EvalState & state) override;

private:
    Expression *exp;

};

/*
 * Class: Print
 * ------------
 * This statement print the value of the expression on
 * the console and then print a newline character.
 */

class Print: public Statement {

public:
    explicit Print(TokenScanner &scanner);
    ~Print() override;
    int execute(EvalState &state) override;

private:
    Expression *exp;
};

/*
 * Class: Input
 * --------------
 * This statement print a prompt consisting of the
 * string " ? " and then to read in a value to be
 * stored in the variable.
 */

class Input: public Statement {

public:
    explicit Input(TokenScanner &scanner);
    ~Input() override;
    int execute(EvalState & state) override;
private:
    string var;
};

/*
 * Class: End
 * --------------
 * This statement marks the end of the program.
 * Execution halts when this line is reached.
 * Execution also stops if the program continues
 * past the last numbered line.
 */

class End: public Statement {

public:
    explicit End(TokenScanner &scanner);
    ~End() override;
    int execute(EvalState & state) override;
};

/*
 * Class: Transfer
 * ---------------
 * This statement transfers control unconditionally
 * to line n in the program. If line n does not exist,
 * your BASIC interpreter should generate an error
 * message informing the user of that fact.
 */

class Transfer: public Statement {

public:
    explicit Transfer(TokenScanner &scanner);
    ~Transfer() override;
    int execute(EvalState &state) override;

private:
    int lineNum;
};

/*
 * Class: Condition
 * ----------------
 * This statement performs a conditional transfer of
 * control. On encountering such a statement, the BASIC
 * interpreter begins by evaluating condition, which in
 * the minimal version of BASIC consists of two arithmetic
 * expressions joined by one of the operators <, >, or =.
 * If the result of the comparison is true, control passes
 * to line n, just as in the GOTO statement; if not, the
 * program continues with the next line in sequence.
 */

class Condition: public Statement {

public:
    explicit Condition(TokenScanner &scanner);
    ~Condition() override;
    int execute(EvalState &state) override;

private:
    Expression *lhs, *rhs;
    string op;
    int lineNum;
};

#endif
