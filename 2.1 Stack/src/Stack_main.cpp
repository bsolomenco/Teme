#include <cstdio>
#include "Stack.hpp"

//================================================================================
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    Stack stack;
    int n = 0;
    printf("number of elements: ");
    scanf("%d", &n);
    printf("elements (SPACE or EOL separated):\n");

    for(int element=0; n--; ){
        scanf("%d", &element);
        stack.push({element});
    }

    printf("popping elements from stack:\n");
    while(!stack.empty()){
        auto element = stack.top();
        printf("    %d\n", element);
        stack.pop();
    }

    return 0;
}