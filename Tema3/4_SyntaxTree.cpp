#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>

//--------------------------------------------------------------------------------
enum ChType{
    NUL,    //invalid
    WSP,    //whitespace
    DIG,    //digit
    DOT,    //decimal point
    P0,     //left paren
    P1,     //right paren
};
enum Op{ADD=10, SUB, MUL, DIV, POW, _1};

//--------------------------------------------------------------------------------
int chType(int ch){
    static int a[256] = {};
    if(a[' '] != ChType::WSP){//initialize
        //operators
        a['+'] = Op::ADD;
        a['-'] = Op::SUB;
        a['*'] = Op::MUL;
        a['/'] = Op::DIV;
        a['^'] = Op::POW;

        a['('] = ChType::P0;
        a[')'] = ChType::P1;

        //whitespaces
        a[' '] = ChType::WSP;
        a['\n'] = ChType::WSP;
        a['\r'] = ChType::WSP;
        a['\t'] = ChType::WSP;

        //digits
        for(int i='0'; i<='9'; ++i){
            a[i] = ChType::DIG;
        }
    }
    return a[ch];
}

//--------------------------------------------------------------------------------
Op getOp(char ch){
    switch(chType(ch)){
        case Op::ADD:
        case Op::SUB:
        case Op::MUL:
        case Op::DIV:
        case Op::POW:
            return (Op)(chType(ch));
        default:
            return (Op)(0);
    }
}

//--------------------------------------------------------------------------------
int precedence(Op op){
    static constexpr int _precedence[Op::_1-Op::ADD] = {2, 2, 3, 3, 4};
    return _precedence[op - Op::ADD];
}

//--------------------------------------------------------------------------------
enum Associativity{LEFT, RIGHT};
Associativity associativity(Op op){
    static const Associativity _associativity[Op::_1-Op::ADD] = {LEFT, LEFT, LEFT, LEFT, RIGHT};
    return _associativity[op - Op::ADD];
}

//--------------------------------------------------------------------------------
struct Node{//tree node
    int type = 0;
    char val = 0;
    Node* parent = 0;
    Node* left   = 0;
    Node* right  = 0;
};

//--------------------------------------------------------------------------------
struct SyntaxTree{
    Node* root = 0;

    SyntaxTree(const char* expr){//build the tree from an arithmetic expression using Shunting-yard algorithm
        char errMsg[1024] = {};
        std::stack<char> opStack;
        std::stack<Node*> nodeStack;
        int prevType = 0;
        const char* p = expr;
        for( ; *p; ++p){
            char currType = chType(*p);
            switch(currType){
                case ChType::NUL:
                    sprintf(errMsg, "%*s^ ERR invalid character (%c)", (int)(p-expr), "", *p);
                    return;
                case ChType::WSP:
                    continue;//ignore whitespaces
                case ChType::DIG:{
                    Node* n = new Node{ChType::DIG, *p};
                    nodeStack.push(n);
                    if (currType == prevType) {
                        sprintf(errMsg, "%*s^ ERR two consecutive operands", (int)(p-expr), "");
                        return;
                    }
                    break;
                }
                case Op::ADD:
                case Op::SUB:
                case Op::MUL:
                case Op::DIV:
                case Op::POW:{
                    if(currType == prevType){
                        sprintf(errMsg, "%*s^ ERR two consecutive operators", (int)(p-expr), "");
                        return;
                    }
                    Op op = (Op)(currType);
                    while(!opStack.empty()){
                        char ch = opStack.top();
                        Op stackOp = getOp(ch);
                        if(stackOp == 0){
                            break;
                        }
                        if(precedence(op)<precedence(stackOp) || (precedence(op)==precedence(stackOp) && associativity(op)==0)){
                            buildSubtreeFromOp(nodeStack, new Node{stackOp, ch});
                            opStack.pop();
                        }else{
                            break;
                        }
                    }
                    opStack.push(*p);
                    break;
                }
                case ChType::P0:
                    opStack.push(*p);
                    break;
                case ChType::P1:
                    while(!opStack.empty() && opStack.top()!='('){
                        char ch = opStack.top();
                        Op stackOp = getOp(ch);
                        buildSubtreeFromOp(nodeStack, new Node{stackOp, ch});
                        opStack.pop();
                    }
                    if(!opStack.empty() && opStack.top()=='('){
                        opStack.pop();
                    }
                    else{
                        sprintf(errMsg, "%*s^ ERR mismatched parentheses: too many ')'", (int)(p-expr), "");
                        return;
                    }
                    break;
                default:
                    printf("ERR\n");
            }
            prevType = currType;
        }
        while(!opStack.empty()) {
            if(opStack.top() == '('){
                sprintf(errMsg, "%*s^ ERR mismatched parentheses: missing ')'", (int)(p-expr), "");
                return;
            }
            char ch = opStack.top();
            buildSubtreeFromOp(nodeStack, new Node{getOp(ch), ch});
            opStack.pop();
        }
        root = nodeStack.top();
        nodeStack.pop();
    }

    //--------------------------------------------------------------------------------
    void buildSubtreeFromOp(std::stack<Node*>& stack, Node* node){
        if(stack.size() < 2){
            printf("ERR not enough operands\n");
            return;
        }
        node->right = stack.top();
        node->right->parent = node;
        stack.pop();
        node->left = stack.top();
        node->left->parent = node;
        stack.pop();
        stack.push(node);
    }

    //--------------------------------------------------------------------------------
    void printNode(Node* node, int level){
        printf("%*s%c\n", 4*level, "", node->val);
        if(node->left)
            printNode(node->left, level+1);
        if(node->right)
            printNode(node->right, level+1);
    }

    //--------------------------------------------------------------------------------
    void printPreOrderTraversalWithLevels(){
        printf("pre-order (transposed):\n");
        for(int i=0; i<10; ++i){
            printf("%-4d", i);
        }
        printf(" <-- tree levels\n");
        printNode(root, 0);
    }

    //--------------------------------------------------------------------------------
    void printBreadthFirstTraversalWithLevels(){
        printf("breadth first traversal with levels:\n");
        int level = -1;
        std::queue<Node*> queue;
        queue.push(root);
        while(queue.empty() == false){
            Node* node = queue.front();
            queue.pop();

            //calculate level of the current node
            int lvl = 0;
            for(Node*n=node; n->parent; n=n->parent){++lvl;}
            if(lvl != level){
                level = lvl;
                printf("\nlevel %d:", lvl);
            }
            printf(" %c", node->val);

            if(node->left)
                queue.push(node->left);
            if(node->right)
                queue.push(node->right);
        }
        printf("\n");
    }

    //--------------------------------------------------------------------------------
    double evalRecursive(Node* node){
        double val0 = 0;
        double val1 = 0;
        if(node->left)
            val0 = evalRecursive(node->left);
        if(node->right)
            val1 = evalRecursive(node->right);
        switch(node->type){
            case ChType::DIG:
                return node->val-'0';
            case Op::ADD:
                return val0+val1;
            case Op::SUB:
                return val0-val1;
            case Op::MUL:
                return val0*val1;
            case Op::DIV:
                return (double)val0/(double)val1;
            default:
                printf("ERR unknown node type\n");
                return 0;
        }
    }

    //--------------------------------------------------------------------------------
    double evalIterative(){//use 2 stacks for post-order traversal (=RPN) then evaluate
        //build postorder-traversal in stack1
        std::stack<Node*> stack0;
        std::stack<Node*> stack1;
        stack0.push(root);
        while(stack0.empty() == false){
            Node* node = stack0.top();
            stack0.pop();
            stack1.push(node);
            if(node->left)
                stack0.push(node->left);
            if(node->right)
                stack0.push(node->right);
        }
        //stack1 gives RPN => eval
        std::stack<double> evalStack;
        printf("RPN:");
        while(stack1.empty() == false){
            Node* node = stack1.top();
            stack1.pop();
            printf(" %c", node->val);
            if(node->type==ChType::DIG){
                evalStack.push(node->val - '0');
            }else{
                if(evalStack.size() < 2) {
                    printf("ERR not enough operands\n");
                    return DBL_MAX;
                }
                double rightOperand = evalStack.top();
                evalStack.pop();
                double leftOperand = evalStack.top();
                evalStack.pop();
                switch(node->type){
                    case Op::ADD:
                        evalStack.push(leftOperand+rightOperand);
                        break;
                    case Op::SUB:
                        evalStack.push(leftOperand-rightOperand);
                        break;
                    case Op::MUL:
                        evalStack.push(leftOperand*rightOperand);
                        break;
                    case Op::DIV:
                        evalStack.push((double)leftOperand/(double)rightOperand);
                        break;
                    default:
                        printf("ERR unknown node type\n");
                        return 0;
                }
            }
        }
        printf("\n");
        return evalStack.top();
    }
};

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("4_SyntaxTree.test", "rt");
    char buf[1024] = {};
    while(fgets(buf, sizeof(buf), file)){
        printf("================================================================\n");
        printf("expression: %s", buf);//input expression
        SyntaxTree syntaxTree(buf);
        syntaxTree.printPreOrderTraversalWithLevels();
        syntaxTree.printBreadthFirstTraversalWithLevels();
        double val = syntaxTree.evalRecursive(syntaxTree.root);
        printf("evalRecursive = %lf\n", val);
        val = syntaxTree.evalIterative();
        printf("evalIterative = %lf\n", val);
    }
    fclose(file);

    return 0;
}