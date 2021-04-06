#include "C_isatty.h"
#include "Rpn.hpp"
#include <cstdio>
#include <limits>

//--------------------------------------------------------------------------------
void showHelp(){
    printf(
        "usage : Rpn < inputFile\n"
        "input file syntax: arithmetical expression (one per line), like:\n"
        "    1 + 2 - 3\n"
        "    3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3\n"
        "    2 + + + 3 ==> ERR\n"
        "    2 * ((3 + 4) ==> ERR\n"
        "    23#+a ==> ERR\n"
        "    3 * 4 - 3 * (24 - 12) - 7 ==> ERR\n"
        "NOTE: works only for single digit numbers!!!\n"
    );
}

//================================================================================
int main([[maybe_unused]] int argc, [[maybe_unused]] char**argv){
    auto redirectedStdin = !isatty(STDIN_FILENO);
    if(!redirectedStdin){//console input not redirected => display help message
        showHelp();
        return 1;
    }
    Rpn rpn;
    char buf[1024] = {};
    while(fgets(buf, sizeof(buf), stdin)){
        printf("%s", buf);//input expression
        if(rpn.infix2postfix(buf)){
            printf("%s\n", rpn.errMsg);
        }else{
            auto val = rpn.eval();
            if(val == std::numeric_limits<double>::infinity()){
                printf("%s\n", rpn.errMsg);
            }else{
                printf("    RPN: %s = %lf\n", rpn.postfix, rpn.eval());
            }
        }
    }

    return 0;
}