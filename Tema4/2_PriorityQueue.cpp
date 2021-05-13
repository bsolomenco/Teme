#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//----------------------------------------------------------------
struct PriorityQueue{
    std::vector<int> heapMax;

    bool empty(){
        return heapMax.empty();
    }

    void siftDown(int index){//pushes node[index] down towards end; assumes root.left and root.right are heaps
        int val = heapMax[index];
        for(int child=2*index+1; child<heapMax.size(); child=2*index+1){
            if(child+1<heapMax.size() && heapMax[child]<heapMax[child+1]){//right child exists and is bigger
                ++child;
            }
            if(val >= heapMax[child]){//in order
                break;
            }
            heapMax[index] = heapMax[child];
            index = child;
        }
        heapMax[index] = val;
    }

    void siftUp(int index){//pushes node up towards start; assumes start is a heap
        int value = heapMax[index];
        for(int parent=(index-1)/2; 0<index && heapMax[parent]<heapMax[index]; parent=(index-1)/2){
                heapMax[index] = heapMax[parent];
                index = parent;
        }
        heapMax[index] = value;
    }

    void push(int val){
        if(heapMax.size() == 0){
            heapMax.push_back(val);
            return;
        }
        heapMax.push_back(0);
        increase(heapMax.size()-1, val);
    }

    void pop(){
        if(heapMax.size() > 1){
            heapMax[0] = heapMax[heapMax.size()-1];
            heapMax.resize(heapMax.size()-1);
            siftDown(0);
        }else{
            heapMax.clear();
        }
    }

    int top(){
        return heapMax[0];
    }

    void increase(int index, int newValue){
        if(newValue > heapMax[index]){
            for(int parent=(index-1)/2; 0<index && heapMax[parent]<newValue; parent=(index-1)/2){
                heapMax[index] = heapMax[parent];
                index = parent;
            }
            heapMax[index] = newValue;
        }
    }

    void replace(int index, int newValue){
        if(newValue < heapMax[index]){
            heapMax[index] = newValue;
            siftDown(index);
        }else{
#if 0
            heapMax[index] = newValue;
            siftUp()
#else
            increase(index, newValue);
#endif
        }
    }

    const char* str(){
        static char buf[1024] = {};
        int n = 0;
        buf[0] = 0;
        for(int i: heapMax){
            n += sprintf(buf+n, "%4d ", i);
        }
        return buf;
    }
};

//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(std::string str){
    std::vector<int> vec;
    int value = 0;
    std::istringstream iss(str);
    while(iss >> value){
        vec.push_back(value);
    }
    return vec;
}

//----------------------------------------------------------------
void help(){
    printf(
"Menu\n"
"    h or ?             this help\n"
"    a val [val]        add value(s)\n"
"    p                  pop\n"
"    r index val        replace: [index]=val\n"
"    s                  show\n"
"    t                  top\n"
"    q or x             quit|exit\n"
    );
}

//================================================================
int main(){
    PriorityQueue pq;
    help();
    char op = 0;
    do{
        printf(">");
        std::cin >> op;
        switch(op){
            case 'a':{
                std::string line;
                std::getline(std::cin, line);
                std::vector<int> v = parseLineOfNumbers(line);
                for(int i : v){
                    pq.push(i);
                }
                break;
            }
            case 'p':
                if(!pq.empty()){
                    pq.pop();
                }
                break;
            case 'r':{
                int index = 0;
                int val = 0;
                std::cin >> index;
                std::cin >> val;
                pq.replace(index, val);
                break;
            }
            case 's':
                printf("priorityQueue={%s}\n", pq.str());
                break;
            case 't':
                printf("top=%d\n", pq.top());
                break;
            case 'q':
            case 'x':
                op = 0;
                break;
            case '?':
            case 'h':
            default:
                help();
        }
    }while(op);
    return 0;
}