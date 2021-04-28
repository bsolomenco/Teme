#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

//--------------------------------------------------------------------------------
struct Node{//tree node
    char ch = 0;
    Node* left   = 0;
    Node* right  = 0;
};

//================================================================================
struct HuffmanTree{
    Node root{'?'};

    //--------------------------------------------------------------------------------
    HuffmanTree(FILE* file){
        char buf0[256] = {0};
        char buf1[256] = {0};
        while(fscanf(file, "%s %s", buf0, buf1) == 2){
            char c = buf0[0];
            Node*node = &root;
            for(char*p=buf1; *p; ++p){
                if(*p == '0'){
                    if(node->left == 0){
                        node->left = new Node{'?'};
                    }
                    node = node->left;
                }else{
                    if(node->right == 0){
                        node->right = new Node{'?'};
                    }
                    node = node->right;
                }
            }
            node->ch = c;
        }
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
        for(stack.push({"", &root, 0, ' '}); stack.empty()==false; ){
            Info info = stack.top();
            stack.pop();
            printf("%s%c--(%c)\n", info.prefix.c_str(), info.type, info.node->ch);
            std::string pfx = info.prefix + (info.type=='+'?"|   ":"    ");
            if(info.node->right)
                stack.push({pfx, info.node->right, info.level+1, '\\'});
            if(info.node->left)
                stack.push({pfx, info.node->left, info.level+1, '+'});
        }
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
        printf("%s%c--(%c)\n", prefix.c_str(), ch[nt], node->ch);
        if(node->left)
            printRightRootLeftRotatedRecursive(prefix+(nt==NodeType::RIGHT?"|   ":"    "), node->left, NodeType::LEFT);
    }
    void printRightRootLeftRotatedRecursive(){
        printf("tree (recursive right->root->left rotated):\n");
        printLevels();
        printRightRootLeftRotatedRecursive("", &root, NodeType::ROOT);
    }
};

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("5_Huffman.test", "rt");
    if(file){
        HuffmanTree huffmanTree(file);
        huffmanTree.printPreorderTransposedIterative();
        huffmanTree.printRightRootLeftRotatedRecursive();
        fclose(file);
    }
    return 0;
}