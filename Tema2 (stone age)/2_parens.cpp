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
int main(int argc,  char** argv){
    printf(
        "input one parenthesis expression per line, like:\n"
        "        ()\n"
        "        {[()]}\n"
        "        ...\n"
        "exit: Ctrl+Z followed by RETURN\n"
    );

    std::stack<char> charStack;
    char buf[1024] = {};
    int err = 0;
    while(fgets(buf, sizeof(buf), stdin)){
        int len = strlen(buf);
        int i = 0;
        for(; i<len; ++i){
            if(buf[i]=='('){
                charStack.push(buf[i]);
            }else if(buf[i]=='[' || buf[i]=='{'){
                if(!charStack.empty() && charStack.top()<buf[i]){//a '[' cannot be inside "(...)" and a '{' cannot be inside "(...)" or "[...]"
                    printf("%*s^ ERR nesting rank: %c %c\n", i, "", charStack.top(), buf[i]);
                    err = 1;
                    break;
                }
                charStack.push(buf[i]);
            }else if(buf[i]==')' || buf[i]==']' || buf[i]=='}'){
                if(charStack.empty() || charStack.top()!=openParen(buf[i])){//closing paren without opening pair
                    printf("%*s^ ERR closing paren '%c' without opening pair\n", i, "", buf[i]);
                    err = 1;
                    break;
                }
                charStack.pop();
            }else{
                //ignore any other chars
            }
        }
        if(!err){
            if(charStack.empty()){
                printf("    OK\n");
            }else{
                printf("%*s^ ERR missing closing pair for '%c'\n", i-1, "", charStack.top());
            }
        }
        charStack = {};
    }
    return 0;
}