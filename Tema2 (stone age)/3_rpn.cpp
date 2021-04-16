#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cmath>
#include <stack>
#pragma warning(disable : 4996)

struct Rpn {
    char postfix[1024] = {};
    char errMsg[1024] = {};

    Rpn();

    int infix2postfix(const char* buf);//result will be in "postfix[]"
    double eval();//evaluate the content of "postfix[]"

//data members
    enum ChType {
        NUL,    //invalid
        WSP,    //whitespace
        DIG,    //digit
        DOT,    //decimal point
        P0,     //left paren
        P1,     //right paren
    };
    enum Op { ADD = 10, SUB, MUL, DIV, POW, _1 };

    int     _chType[128] = {};

    Op getOp(char ch);
    int precedence(Op op);

    enum Associativity { LEFT, RIGHT };
    Associativity associativity(Op op);
};

namespace {
    double add(double x, double y) { return x + y; }
    double sub(double x, double y) { return x - y; }
    double mul(double x, double y) { return x * y; }
    double div(double x, double y) { return x / y; }
    double pow(double x, double y) { return std::pow(x, y); }

    using Func = double(*)(double, double);
    Func func[] = { &add, &sub, &mul, &div, &pow };
}

//--------------------------------------------------------------------------------
Rpn::Rpn() {
    //operators
    _chType['+'] = Op::ADD;
    _chType['-'] = Op::SUB;
    _chType['*'] = Op::MUL;
    _chType['/'] = Op::DIV;
    _chType['^'] = Op::POW;

    _chType['('] = ChType::P0;
    _chType[')'] = ChType::P1;

    //whitespaces
    _chType[' '] = ChType::WSP;
    _chType['\n'] = ChType::WSP;
    _chType['\r'] = ChType::WSP;
    _chType['\t'] = ChType::WSP;

    //digits
    for (int i = '0'; i <= '9'; ++i) {
        _chType[i] = ChType::DIG;
    }

    //_chType['.'] = ChType::DOT;//not implemented
}

//--------------------------------------------------------------------------------
int Rpn::infix2postfix(const char* buf) {
    //using Shunting-yard algorithm
    char* dst = postfix;
    *dst = 0;
    std::stack<char> stack;
    int prevType = 0;
    const char* p = buf;
    for (; *p; ++p) {
        char currType = _chType[*p];
        switch (currType) {
        case ChType::NUL:
            sprintf(errMsg, "%*s^ ERR invalid character (%c)", (int)(p - buf), "", *p);
            return 1;
        case ChType::WSP:
            continue;//ignore whitespaces
        case ChType::DIG:
            *dst = *p;
            *++dst = 0;
            if (currType == prevType) {
                sprintf(errMsg, "%*s^ ERR two consecutive operands", (int)(p - buf), "");
                return 2;
            }
            break;
        case Op::ADD:
        case Op::SUB:
        case Op::MUL:
        case Op::DIV:
        case Op::POW: {
            if (currType == prevType) {
                sprintf(errMsg, "%*s^ ERR two consecutive operators", (int)(p - buf), "");
                return 3;
            }
            Op op = (Op)(currType);
            while (!stack.empty()) {
                char ch = stack.top();
                Op stackOp = getOp(ch);
                if (stackOp == 0) {
                    break;
                }
                if (precedence(op) < precedence(stackOp) || (precedence(op) == precedence(stackOp) && associativity(op) == 0)) {
                    *dst = stack.top();
                    *++dst = 0;
                    stack.pop();
                }
                else {
                    break;
                }
            }
            stack.push(*p);
            break;
        }
        case ChType::P0:
            stack.push(*p);
            break;
        case ChType::P1:
            while (!stack.empty() && stack.top() != '(') {
                *dst = stack.top();
                *++dst = 0;
                stack.pop();
            }
            if (!stack.empty() && stack.top() == '(') {
                stack.pop();
            }
            else {
                sprintf(errMsg, "%*s^ ERR mismatched parentheses: too many ')'", (int)(p - buf), "");
                return 4;
            }
            break;
        default:
            printf("ERR\n");
        }
        prevType = currType;
    }
    while (!stack.empty()) {
        if (stack.top() == '(') {
            sprintf(errMsg, "%*s^ ERR mismatched parentheses: missing ')'", (int)(p - buf), "");
            return 5;
        }
        *dst = stack.top();
        *++dst = 0;
        stack.pop();
    }
    return 0;
}

//--------------------------------------------------------------------------------
double Rpn::eval() {
    std::stack<double> stack;
    for (char ch : postfix) {
        switch (_chType[ch]) {
            case 0:
                return stack.top();
            case ChType::DIG:
                stack.push(ch - '0');
                break;
            case Op::ADD:
            case Op::SUB:
            case Op::MUL:
            case Op::DIV:
            case Op::POW: {
                if (stack.size() < 2) {
                    sprintf(errMsg, "ERR not enough operands");
                    return DBL_MAX;
                }
                double y = stack.top();
                stack.pop();
                double x = stack.top();
                stack.pop();
                stack.push((*func[_chType[ch] - Op::ADD])(x, y));
                break;
            }
        }
    }
    return stack.top();
}

//--------------------------------------------------------------------------------
Rpn::Op Rpn::getOp(char ch) {
    switch (_chType[ch]) {
    case Op::ADD:
    case Op::SUB:
    case Op::MUL:
    case Op::DIV:
    case Op::POW:
        return (Op)(_chType[ch]);
    default:
        return (Op)(0);
    }
}

//--------------------------------------------------------------------------------
int Rpn::precedence(Op op) {
    static constexpr int _precedence[Op::_1 - Op::ADD] = { 2, 2, 3, 3, 4 };
    return _precedence[op - Op::ADD];
}

//--------------------------------------------------------------------------------
Rpn::Associativity Rpn::associativity(Op op) {
    static constexpr Associativity _associativity[Op::_1 - Op::ADD] = { LEFT, LEFT, LEFT, LEFT, RIGHT };
    return _associativity[op - Op::ADD];
}

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("3_rpn.test","rt");
    Rpn rpn;
    char buf[1024] = {};
    while(fgets(buf, sizeof(buf), file)){
        printf("%s", buf);//input expression
        if(rpn.infix2postfix(buf)){
            printf("%s\n", rpn.errMsg);
        }else{
            double val = rpn.eval();
            if(val == DBL_MAX){
                printf("%s\n", rpn.errMsg);
            }else{
                printf("    RPN: %s = %lf\n", rpn.postfix, rpn.eval());
            }
        }
    }
    fclose(file);
    return 0;
}