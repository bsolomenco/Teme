#include "C_isatty.h"
#include <cstdio>
#include <stack>

//--------------------------------------------------------------------------------
char openParen(char closeParen){//returns the corresponding open paren for the given close paren
    switch(closeParen){
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        default : return 0;
    }
}

//================================================================================
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    auto redirectedStdin = !isatty(STDIN_FILENO);
    if(!redirectedStdin){//console input (not redirected)
        printf(
            "usage (file input)     : Parens < inputFile\n"
            "usage (console input)  : Parens\n"
            "input syntax: one parenthesis expression per line with ranking: {} > [] > (), like:\n"
            "        ()\n"
            "        {[()]}\n"
            "        ...\n"
            "exit: Ctrl+Z followed by RETURN\n"
        );
    }

    std::stack<char> charStack;
    char buf[1024] = {};
    while(fgets(buf, sizeof(buf), stdin)){
        if(buf[0]=='\n' || redirectedStdin && (buf[0]=='#')){//skip comments & empty lines
            continue;
        }
        auto len = strlen(buf);
        int i = 0;
        for(; i<len; ++i){
            switch(buf[i]){
                case '(':
                    charStack.push(buf[i]);
                    break;
                case '[':
                    [[fallthrough]];
                case '{':
                    if(!charStack.empty() && charStack.top()<buf[i]){//a '[' cannot be inside "(...)" and a '{' cannot be inside "(...)" or "[...]"
                        redirectedStdin && printf(buf);
                        printf("%*s^ ERR nesting rank: %c %c\n", i, "", charStack.top(), buf[i]);
                        goto afterFor;
                    }
                    charStack.push(buf[i]);
                    break;
                case ')':
                    [[fallthrough]];
                case ']':
                    [[fallthrough]];
                case '}':
                    if(charStack.empty() || charStack.top()!=openParen(buf[i])){//closiong paren withot opening pair
                        redirectedStdin && printf(buf);
                        printf("%*s^ ERR closing paren '%c' without opening pair\n", i, "", buf[i]);
                        goto afterFor;
                    }
                    charStack.pop();
                    break;
                default:
                    ;//ignore any other chars
            }
        }
        redirectedStdin && printf(buf);
        if(charStack.empty()){
            printf("    OK\n");
        }else{
            printf("%*s^ ERR missing closing pair for '%c'\n", i-1, "", charStack.top());
        }
afterFor:
        charStack = {};
    }
    return 0;
}