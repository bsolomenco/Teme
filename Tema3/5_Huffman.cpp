#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>

//--------------------------------------------------------------------------------
struct Node{//tree node
    char c = 0;
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
            node->c = c;
        }
    }

    //--------------------------------------------------------------------------------
    void printNode(Node* node, int level){
        printf("%*s%c\n", 4*level, "", node->c);
        if(node->left)
            printNode(node->left, level+1);
        if(node->right)
            printNode(node->right, level+1);
    }

    //--------------------------------------------------------------------------------
    void print(){
        printf("Huffman tree (transposed):\n");
        for(int i=0; i<10; ++i){
            printf("%-4d", i);
        }
        printf(" <-- tree levels\n");
        printNode(&root, 0);
    }
};

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("5_Huffman.test", "rt");
    if(file){
        HuffmanTree huffmanTree(file);
        huffmanTree.print();
        fclose(file);
    }
    return 0;
}