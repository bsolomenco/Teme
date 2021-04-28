#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>

//----------------------------------------------------------------
char* getFirstNonWhitespace(char* buf){
    for(char*p=buf; *p; ++p){
        if(!isspace(*p)){
            return p;
        }
    }
    return buf;
}

//----------------------------------------------------------------
void trimRight(char* buf){
    for(int i=strlen(buf); --i>=0 && isspace(buf[i]); buf[i]=0);
}
char* trim(char* buf){
    trimRight(buf);
    return getFirstNonWhitespace(buf);
}


//================================================================
struct Tree{
    struct Node{//tree node
        std::string        name;
        std::vector<Node*> kids;
    };
    Node root;

    //----------------------------------------------------------------
    void load(FILE* file){
        struct Aux{
            Node* node = 0;
            int   level = 0;
        };
        char buf[1024] = {};
        if(!fgets(buf, sizeof(buf), file)){
            printf("ERR loading file\n");
            return;
        }
        if(isspace(buf[0])){
            printf("ERR invalid file content: 1st line must not start with whitespace(s)\n");
            return;
        }
        trimRight(buf);
        root.name = buf;
        std::stack<Aux> stack;
        stack.push({&root});
        for(int line=0; fgets(buf, sizeof(buf), file); ++line){
            Aux& aux = stack.top();
            trimRight(buf);
            char* name = getFirstNonWhitespace(buf);
            int level = (name-buf)/4;
            if(level > aux.level){
                stack.push({new Node{name}, aux.level+1});
                aux.node->kids.push_back(stack.top().node);
                continue;
            }
            do{
                stack.pop();
                aux = stack.top();
            }while(level <= aux.level);
            Node* node = new Node{name};
            aux.node->kids.push_back(node);
            stack.push({node, aux.level+1});
        }
    }

    //--------------------------------------------------------------------------------
#if 0 //output
- node info could be any length => need to be printed only one per line (otherwise the layout could be either destroyed or become very big)
- this solution traverse in-order and indent each node proportional with its tree level
0
+---1
|   +---2
|   |   +---21
|   |   +---22
|   |   \---23 (last kid)
|   \---3
|       \---31 (only one kid)
\---4
    +---41
    \---42
#endif
    void printPreorderTransposedIterative(){
        printf("tree (iterative preorder transposed levels; +=non-last \\=last):\n");
        if(root.name.empty()){
            return;
        }
        struct Info{
            std::string prefix;
            Node*       node = 0;
            int         level = 0;
            char        type = 0;//' '=root '|'=left '\'=right
            Node*       parent = 0;
        };
        std::stack<Info> stack;
        for(stack.push({"", &root, 0, ' '}); stack.empty()==false; ){
            Info info = stack.top();
            stack.pop();
            printf("%s%c---%s [parent=%s kids=%d]\n", info.prefix.c_str(), info.type, info.node->name.c_str(), info.parent?info.parent->name.c_str():"none", info.node->kids.size());
            std::string pfx = info.prefix + (info.type=='+'?"|   ":"    ");
            int i = info.node->kids.size();
            if(--i >= 0)
                stack.push({pfx, info.node->kids[i], info.level+1, '\\', info.node});
            while(--i >= 0){
                stack.push({pfx, info.node->kids[i], info.level+1, '+', info.node});
            }
        }
    }

    //--------------------------------------------------------------------------------
    void getInfo(const char* name){//using breadth-first search
        struct Info{
            Node* node = 0;
            int level = 0;
        };
        if(root.name.empty()){
            return;
        }
        int maxLevel = 0;
        std::queue<Info> queue;
        queue.push({&root, 0});
        while(queue.empty() == false){
            Info info = queue.front();
            queue.pop();
            for(Node*node : info.node->kids){
                if(node->name == name){
                    printf("    parent: %s\n", info.node->name.c_str());
                    printf("    kid(s):");
                    for(Node* kid : node->kids){
                        printf(" %s", kid->name.c_str());
                    }
                    printf("\n");
                    return;
                }
                queue.push({node, info.level+1});
            }
        }
        printf("%s NOT FOUND\n", name);
    }

    //--------------------------------------------------------------------------------
    int depth(){//using breadth-first search
        struct Info{
            Node* node = 0;
            int level = 0;
        };
        if(root.name.empty()){
            return 0;
        }
        int maxLevel = 0;
        std::queue<Info> queue;
        queue.push({&root, 0});
        while(queue.empty() == false){
            Info info = queue.front();
            queue.pop();
            if(maxLevel < info.level){
                maxLevel = info.level;
            }
            for(Node*n : info.node->kids){
                queue.push({n, info.level+1});
            }
        }
        return maxLevel+1;
    }
};

//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("8_DescendentsTree.test", "rt");
    if(file){
        Tree tree;
        tree.load(file);
        fclose(file);
        tree.printPreorderTransposedIterative();

        printf("generations (tree depth) = %d\n", tree.depth());

        printf("person to search for (press RETURN to exit):\n");
        char buf[1024] = {};
        while(fgets(buf, sizeof(buf), stdin)){
            char* p = trim(buf);
            if(*p == 0){
                break;
            }
            tree.getInfo(p);
        }
    }
    return 0;
}