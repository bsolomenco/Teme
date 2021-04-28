#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>

//================================================================================
struct Tree{
    struct Node{//tree node
        int id = 0;         //identifier
        Node* left   = 0;   //left child
        Node* right  = 0;   //right child
    };
    Node* root = 0;

    //--------------------------------------------------------------------------------
    Tree(std::vector<int>& preorder, std::vector<int>& inorder){
        struct Info{//rebuild information
            int p0=0, p1=0; //preorder lower & upper bounds
            int i0=0, i1=0; //inorder  lower & upper bounds
            Node * node = 0;//current node
        };
        std::stack<Info> stack;
        root = new Node{preorder[0]};
        stack.push({0, (int)preorder.size()-1, 0, (int)inorder.size()-1, root});
        while(stack.empty() == false){
            Info info = stack.top();
            stack.pop();
            for(int i=info.i0; i<=info.i1; ++i){
                if(inorder[i] == preorder[info.p0]){
                    if(i < info.i1){//we have something in the right subtree
                        int len = info.i1-i;
                        int p0 = info.p1-len+1;
                        info.node->right = new Node{preorder[p0]};
                        stack.push({p0, info.p1, i+1, info.i1, info.node->right});//preorder & inorder traversals for the right subtree of the current node
                    }
                    if(info.i0 < i){//we have something in the left subtree
                        int len = i-info.i0;
                        info.node->left = new Node{preorder[info.p0+1]};
                        stack.push({info.p0+1, info.p0+len, info.i0, i-1, info.node->left});//preorder & inorder traversals for the left  subtree of the current node
                    }
                    break;
                }
            }
        }
    }

    //--------------------------------------------------------------------------------
    void printNodesOnEachLevel(){
        printf("tree (breadth-first search + levels):");
        struct Info{
            Node* node = 0;
            int level = 0;
        };
        std::queue<Info> queue;
        queue.push({root, 0});
        for(int level=-1; queue.empty()==false; ){
            Info info = queue.front();
            queue.pop();
            if(info.level != level){
                level = info.level;
                printf("\n    level %2d:", level);
            }
            printf(" %2d", info.node->id);
            if(info.node->left)
                queue.push({info.node->left, level+1});
            if(info.node->right)
                queue.push({info.node->right, level+1});
        }
        printf("\n");
    }

    //--------------------------------------------------------------------------------
    void printLevels(){
        printf("    ");
        for(int i=0; i<10; ++i){
            printf("%-4d", i);
        }
        printf(" <-- tree levels\n");
    }

    //--------------------------------------------------------------------------------
#if 0 //output
- node info could be any length => need to be printed only one per line (otherwise the layout could be either destroyed or become very big)
- this solution traverse in-order and indent each node proportional with its tree level
0
+---1
|   +---2
|   |   +---21 (left node only)
|   \---3
|       \---31 (right node only)
\---4
    +---41 (both left...
    \---42 ...and right nodes)
#endif
    void printPreorderTransposedIterative(){
        printf("tree (iterative preorder transposed levels; +=left \\=right):\n");
        printLevels();
        struct Info{
            std::string prefix;
            Node*       node = 0;
            int         level = 0;
            char        type = 0;//' '=root '|'=left '\'=right
        };
        std::stack<Info> stack;
        for(stack.push({"", root, 0, ' '}); stack.empty()==false; ){
            Info info = stack.top();
            stack.pop();
            printf("%s%c--%2d\n", info.prefix.c_str(), info.type, info.node->id);
            std::string pfx = info.prefix + (info.type=='+'?"|   ":"    ");
            if(info.node->right)
                stack.push({pfx, info.node->right, info.level+1, '\\'});
            if(info.node->left)
                stack.push({pfx, info.node->left, info.level+1, '+'});
        }
    }

    //--------------------------------------------------------------------------------
    void printPreorderTransposedRecursive(std::string prefix, Node* node, bool leftType){
        printf("%s%c--%2d\n", prefix.c_str(), leftType?'+':'\\', node->id);
        std::string pfx = prefix + (leftType ? "|   " : "    " );
        if(node->left)
            printPreorderTransposedRecursive(pfx, node->left, true);
        if(node->right)
            printPreorderTransposedRecursive(pfx, node->right, false);
    }
    void printPreorderTransposedRecursive(){
        printf("tree (recursive preorder transposed levels; +=left \\=right):\n");
        printLevels();
        printPreorderTransposedRecursive("", root, false);
    }

    //--------------------------------------------------------------------------------
#if 0
- this solution traverse right->root->left and indent each node proportional with its tree level
                /--99
            /--98
        /--10
    /--17
    |   \--24
    |       \--32
 --30
    |   /--15
    \--23
        |   /--41
        \-- 8
#endif
    enum NodeType{ROOT, LEFT, RIGHT};
    void printRightRootLeftRotatedRecursive(std::string prefix, Node* node, NodeType nt){
        static char ch[3] = {' ', '\\', '/'};
        if(node->right)
            printRightRootLeftRotatedRecursive(prefix+(nt==NodeType::LEFT?"|   ":"    "), node->right, NodeType::RIGHT);
        printf("%s%c--%2d\n", prefix.c_str(), ch[nt], node->id);
        if(node->left)
            printRightRootLeftRotatedRecursive(prefix+(nt==NodeType::RIGHT?"|   ":"    "), node->left, NodeType::LEFT);
    }
    void printRightRootLeftRotatedRecursive(){
        printf("tree (recursive right->root->left rotated):\n");
        printLevels();
        printRightRootLeftRotatedRecursive("", root, NodeType::ROOT);
    }
};

//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(const char* buf){
    std::vector<int> vec;
    const char* end = buf + strlen(buf);
    int value = 0;
    std::istringstream iss(buf);
    while(iss >> value){
        vec.push_back(value);
    }
    return vec;
}

//--------------------------------------------------------------------------------
void printTraversal(const char* text, const std::vector<int>& traversal){
    printf("%s", text);
    for(int i: traversal){
        printf(" %2d", i);
    }
    printf("\n");
}

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("6_RebuildTree.test", "rt");
    if(file){
        char buf[1024] = {};
        size_t columns = 0;
        std::vector<int> traversals[2];
        for(int line=0; fgets(buf, sizeof(buf), file); ++line){
            if (buf[0] == '\n') {//skip empty lines
                line = 1;
                continue;
            }
            traversals[line%2].clear();
            traversals[line%2] = parseLineOfNumbers(buf);
            if(line%2 == 1){
                if(traversals[0].size() != traversals[1].size()){
                    printf("ERR traversals have different length\n");
                    return 1;
                }
                printf("================================================================\n");
                printTraversal("preorder:", traversals[0]);
                printTraversal("inorder :", traversals[1]);

                Tree tree(traversals[0], traversals[1]);
                tree.printNodesOnEachLevel();
                tree.printPreorderTransposedRecursive();
                tree.printPreorderTransposedIterative();
                tree.printRightRootLeftRotatedRecursive();
            }
        }
    }
    return 0;
}