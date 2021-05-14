#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

//Full priority queue impementation already done in Tema4.2! Using standard one here

//--------------------------------------------------------------------------------
struct Row{
    int index = 0;
    int avail = 0;//number of available seats on row
};
auto compareRow = [](const Row& a, const Row& b){//compare 2 rows by price
    return a.index*a.avail < b.index*b.avail;
};

struct TicketDispenser{
    std::priority_queue<Row, std::vector<Row>, decltype(compareRow)> maxHeap;

    TicketDispenser(std::vector<int> rowAvail)
        : maxHeap(compareRow)
    {
        for(int index=0; index<rowAvail.size(); ++index){
            maxHeap.push({index+1, rowAvail[index]});
        }
    }

    void print(){
        auto q = maxHeap;
        while(!q.empty()){
            Row r = q.top();
            printf(" {i=%d a=%d p=%d}", r.index, r.avail, r.index*r.avail);
            q.pop();
        }
        printf("\n");
    }

    bool empty(){
        return maxHeap.empty();
    }

    Row getMaxPriceTicket(){
        Row row = maxHeap.top();
        int price = row.index*row.avail;
        maxHeap.pop();
        if(row.avail > 1){
            maxHeap.push({row.index, row.avail-1});
        }
        return row;
    }
};

//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(const char* buf) {
    std::vector<int> vec;
    int value = 0;
    std::istringstream iss(buf);
    while(iss >> value){
        vec.push_back(value);
    }
    return vec;
}

//================================================================================
int main(int argc, char**argv){
    char buf[1024] = {};
    FILE* file = fopen("3_MaxGain.test", "rt");
    while(fgets(buf, sizeof(buf), file)){
        std::vector<int> v = parseLineOfNumbers(buf);
        printf("input:");
        for(int i=0; i<v.size(); ++i){printf(" {row=%d capacity=%d}", i+1, v[i]);}
        printf("\n");

        TicketDispenser ticketDispenser(v);
        printf("initial maxHeap (price descending order):");
        ticketDispenser.print();
        int totalGain = 0;
        for(int i=1; !ticketDispenser.empty(); ++i){
            Row row = ticketDispenser.getMaxPriceTicket();
            int price = row.index*row.avail;
            totalGain += price;
            printf("ticket#%d row=%d price=%d => totalGain=%d\n", i, row.index, price, totalGain);
            //ticketDispenser.print();
        }
        printf("\n");
    }
    fclose(file);
    return 0;
}