#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

//--------------------------------------------------------------------------------
struct Median{
    std::priority_queue<int, std::vector<int>, std::less<int>>    lt;//max priority queue with values smaller than median
    std::priority_queue<int, std::vector<int>, std::greater<int>> gt;//min priority queue with values bigger  than median

    Median(){
        lt.push (std::numeric_limits<int>::min());
        gt.push(std::numeric_limits<int>::max());
    }

    void add(int val){
        if(val >= lt.top()){
            gt.push(val);
        }else{
            lt.push(val);
        }
        if(gt.size()-lt.size() == 2){//redistribute one element from bigger values to smaller values
            lt.push(gt.top());
            gt.pop();
        }else if(lt.size()-gt.size() == 2){//redistribute one element from smaller values to bigger values
            gt.push(lt.top());
            lt.pop();
        }
    }

    double get(){
        if(lt.size() == gt.size()){
            return ((double)lt.top()+gt.top())/2.0;
        }else if(lt.size() > gt.size()){
            return lt.top();
        }else{
            return gt.top();
        }
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

//================================================================================
int main(int argc, char**argv){
    char buf[1024] = {};
    FILE* file = fopen("4_Median.test", "rt");
    while(fgets(buf, sizeof(buf), file)){
        Median median;
        std::vector<int> v = parseLineOfNumbers(buf);
        printf("input:");
        for(int i: v){printf(" %d", i);}
        printf("\n");

        printf("incremental median:");
        for(int i: v){
            median.add(i);
            printf(" %.2lf", median.get());
        }
        printf("\n\n");
    }
    fclose(file);

    return 0;
}