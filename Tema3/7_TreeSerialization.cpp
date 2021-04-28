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
        std::string info;   //information
        Node* left   = 0;   //left child
        Node* right  = 0;   //right child
    };
    Node root;

    //--------------------------------------------------------------------------------
    void deserializeFromPreorderTraversal(const char* serializedPreorder){//example ('.' means NULL): "0 10 25 . . 27 . . 15 . 20 33 ."
        struct Aux{
            Node* node = 0;
            int   count = 0;//number of children processed
        };
        std::istringstream iss(serializedPreorder);
        std::string str;
        std::stack<Aux> stack;
        iss >> root.info;
        stack.push({&root});
        while(stack.empty() == false){
            Aux& current = stack.top();
            if(current.node->right){
                stack.pop();
                continue;
            }
            iss >> str;
            if(str == "."){//no child
                current.count++;
                if(current.count == 2){
                    stack.pop();
                    continue;
                }
            }else{
                Node* node = new Node{str};
                if(current.count == 0){
                    current.node->left = node;
                }else{
                    current.node->right = node;
                }
                current.count++;
                stack.push({node});
            }
        }
    }

    //--------------------------------------------------------------------------------
    void deserializeFromLevelTraversal(const char* serializedLevel){//example ('.' means NULL): "1 2 3 . . 4 5"
        std::istringstream iss(serializedLevel);
        std::string str;
        std::queue<Node*> queue;
        iss >> root.info;
        queue.push(&root);
        while(queue.empty() == false){
            Node* current = queue.front();
            queue.pop();
            if(!(iss >> str)){break;}
            if(str != "."){
                current->left = new Node{str};
                queue.push(current->left);
            }
            if(!(iss >> str)){break;}
            if(str != "."){
                current->right = new Node{str};
                queue.push(current->right);
            }
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
        if(root.info.empty()){
            return;
        }
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
            printf("%s%c--%s\n", info.prefix.c_str(), info.type, info.node->info.c_str());
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
        printf("%s%c--%s\n", prefix.c_str(), ch[nt], node->info.c_str());
        if(node->left)
            printRightRootLeftRotatedRecursive(prefix+(nt==NodeType::RIGHT?"|   ":"    "), node->left, NodeType::LEFT);
    }
    void printRightRootLeftRotatedRecursive(){
        printf("tree (recursive right->root->left rotated):\n");
        if(root.info.empty()){
            return;
        }
        printLevels();
        printRightRootLeftRotatedRecursive("", &root, NodeType::ROOT);
    }

    //--------------------------------------------------------------------------------
    int depth(){//using breadth-first search
        struct Info{
            Node* node = 0;
            int level = 0;
        };
        if(root.info.empty()){
            return 0;
        }
        int maxLevel = 0;
        std::queue<Info> queue;
        queue.push({&root, 0});
        while(queue.empty() == false){
            Info info = queue.front();
            queue.pop();
            if(maxLevel < info.level)
                maxLevel = info.level;
            if(info.node->left)
                queue.push({info.node->left, info.level+1});
            if(info.node->right)
                queue.push({info.node->right, info.level+1});
        }
        return maxLevel+1;
    }
};

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("7_TreeSerialization.test", "rt");
    if(file){
        char buf[1024] = {};
        char out[1024] = {};
        for(int counter=0; fgets(buf, sizeof(buf), file); ){
            if(buf[0] == '\n'){//skip empty lines
                continue;
            }
            printf("%d ================================================================\n", ++counter);
            printf("%s", buf);
            Tree binaryTree;
            if(strncmp(buf, "BFS: ", 5) == 0){
                binaryTree.deserializeFromLevelTraversal(buf+5);
            }else if(strncmp(buf, "DFS: ", 5) == 0){
                binaryTree.deserializeFromPreorderTraversal(buf+5);
            }
            printf("tree depth = %d\n", binaryTree.depth());
            binaryTree.printPreorderTransposedIterative();
            binaryTree.printRightRootLeftRotatedRecursive();
        }
    }
    return 0;
}