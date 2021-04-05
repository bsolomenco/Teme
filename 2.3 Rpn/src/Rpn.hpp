#pragma once

class Rpn{
public:
    char postfix[1024] = {};
    char errMsg[1024] = {};

    Rpn();

    int infix2postfix(const char* buf);//result will be in "postfix[]"
    double eval();//evaluate the content of "postfix[]"

private:
    enum ChType{
        NUL,    //invalid
        WSP,    //whitespace
        DIG,    //digit
        DOT,    //decimal point
        P0,     //left paren
        P1,     //right paren
    };
    enum Op{ADD=10, SUB, MUL, DIV, POW, _1};

    int     _chType[128] = {};

    Op getOp(char ch);
    int precedence(Op op);

    enum Associativity{LEFT, RIGHT};
    Associativity associativity(Op op);
};