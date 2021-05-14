#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#if 0
Encoding
- input alphabet: chars/bytes [0..255]
- determine frequency for each letter from alphabet by reading the entire input
- minHeap with pairs {id,fr}, initialized with the letter from alphabet and coresponding frequencies it frequency>0
- work with minHeap to produce Huffman tree (or an equivalent vector constructed in reverse order)
    - initialize table[256][2] = {}
    - pop first 2 minimum frequencies from minHeap: {a,f0}, {b,f1}
    - table.push({a,b})
    - newId = table.size()
    - minHeap.push({newId,f0+f1})
Decoding
#endif

//----------------------------------------------------------------
struct IdFr{//Id & Frequency
    unsigned fr = 0;
    int      id = 0;
};
bool greaterIdFr(const IdFr&a, const IdFr&b){
    return a.fr > b.fr;
}

//================================================================
struct Huffmann{
    struct Node{
        int left = 0;
        int right = 0;
    };

    unsigned char*              buf;
    std::size_t                 len;
    std::vector<unsigned char>  alphabet;
    std::vector<unsigned>       letter2alphabet;//input letter to alphabet index
    std::vector<IdFr>           minHeapVec;     //vector with frequencies, used as minHeap/priority queue
    std::vector<Node>           tree;           //Huffman tree stored as array of pairs {leftChildIndex,rightChildIndex}
    std::vector<std::string>    code;           //Huffman code for each letter from alphabet

    //------------------------------------------------------------
    Huffmann(unsigned char* buf, std::size_t len)
        : buf{buf}
        , len(len)
        , letter2alphabet(256)
    {}

    //------------------------------------------------------------
    void printHeap(){
        printf("minHeap{fr ch}:");
        for(auto i: minHeapVec){
            if(i.id < 0){
                printf(" {%u '%c'}", i.fr, alphabet[-i.id-1]);
            }else{
                printf(" {%u %d}", i.fr, i.id);
            }
        }
        printf("\n");
    }

    //------------------------------------------------------------
    void printState(int val){
        if(val < 0){
            printf("     '%c'", alphabet[-val-1]);
        }else{
            printf("%8d", val);
        }
    }

    //------------------------------------------------------------
    void printTabRow(std::size_t index){
        printf("%d: {", index);
        printState(tree[index].left);
        printState(tree[index].right);
        printf("}\n");
    }

    //------------------------------------------------------------
    void generate(){
        unsigned frequency[256] = {};
        for(std::size_t i=0; i<len; ++i){
            frequency[buf[i]]++;
        }
        for(unsigned i=0; i<256; ++i){
            if(frequency[i] > 0){
                letter2alphabet[i] = alphabet.size();
                alphabet.push_back(i);
            }
        }
        std::sort(alphabet.begin(), alphabet.end());
        printf("frequencies & letters: ");
        for(auto letter: alphabet){
            printf(" {%u '%c'}", frequency[letter], letter);
        }
        printf("\n");

        for(int i=0; i<alphabet.size(); ++i){
            minHeapVec.push_back({frequency[alphabet[i]], -i-1});//leafs have negative id (negative index from alphabet - 1)
        }
        std::make_heap(minHeapVec.begin(), minHeapVec.end(), greaterIdFr);
        printHeap();

        tree.push_back({0,0});//reserve for root
        while(minHeapVec.size() > 1){
            //pop first 2 minimum frequencies from minHeap
            auto left = minHeapVec[0];
            std::pop_heap(minHeapVec.begin(), minHeapVec.end(), greaterIdFr);
            minHeapVec.pop_back();
            auto right = minHeapVec[0];
            std::pop_heap(minHeapVec.begin(), minHeapVec.end(), greaterIdFr);
            minHeapVec.pop_back();

            tree.push_back({left.id, right.id});
            //printTabRow(tree.size()-1);

            int newId = (int)tree.size()-1;
            minHeapVec.push_back({left.fr+right.fr, newId});
            std::push_heap(minHeapVec.begin(), minHeapVec.end(), greaterIdFr);
            //printHeap();
        }
        tree[0] = tree[tree.size()-1];
        tree.pop_back();

        char buf[256] = {};
        printf("non-leaf Huffmann nodes as {leftChildIndex , rightChildIndex}:\n");
        for(std::size_t i=0; i<tree.size(); ++i){
            printTabRow(i);
        }

        //get the codes for each letter
        code.resize(alphabet.size());
        struct NodeInf{
            int index = 0;
            std::string prefix;
        };
        std::stack<NodeInf> stack;
        stack.push({0, ""});
        while(!stack.empty()){
            NodeInf ni = stack.top();
            stack.pop();
            if(ni.index < 0){//leaf
                code[-ni.index-1] = ni.prefix;
            }else{
                stack.push({tree[ni.index].right, ni.prefix+"1"});
                stack.push({tree[ni.index].left, ni.prefix+"0"});
            }
        }
        printf("Huffmann codes:\n");
        for(int i=0; i<alphabet.size(); ++i){
            printf("'%c' <=> %s\n", alphabet[i], code[i].c_str());
        }
    }

    //------------------------------------------------------------
    void encode(){
        for(std::size_t i=0; i<len; ++i){
            int index = letter2alphabet[buf[i]];
            printf("%s", code[index].c_str());
        }
    }
};

//================================================================
int main(int argc, char**argv){
    const char* filename = (argc>1 ? argv[1] : "5_Huffman.test");
    FILE* file = fopen(filename, "rb");
    if(!file){
        printf("ERR cannot open file '%s'\n", filename);
        return 1;
    }
#if 0 //test
    {
        unsigned char buf[] = {"aabcebaa"};
        Huffman huffman(buf, sizeof(buf)-1);
        huffman.generate();
    }
#endif

    unsigned char buf[8192] = {};
    std::size_t len = fread(buf, 1, sizeof(buf), file);
    if(len > 0){
        Huffmann huffmann(buf, len);
        huffmann.generate();
        printf("encoded input:\n");
        huffmann.encode();
        printf("\n");
    }
    fclose(file);
    return 0;
}