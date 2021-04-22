#include <cstdint>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

//--------------------------------------------------------------------------------
struct GrafNeorientat{
    std::vector<std::vector<int>> destinationNodes;
    std::vector<std::vector<int>> sourceNodes;

    struct Inf{
        bool visited = false;
        int  dist    = 0;
    };
    std::vector<Inf> infVec;

    //--------------------------------------------------------------------------------
    void read(const char* filename){
        std::ifstream file(filename);
        std::string line;
        while(std::getline(file, line)){
            std::istringstream iss(line);
            int nodeId = 0;
            iss >> nodeId;
            destinationNodes.push_back({});
            int adiacentNodeId = 0;
            while(iss >> adiacentNodeId){
                destinationNodes[nodeId].push_back(adiacentNodeId);
            }
        }
    }

    //--------------------------------------------------------------------------------
    void print(){
        printf("graf (liste de adiacenta):\n");
        int i = 0;
        for(std::vector<int>& v: destinationNodes){
            printf("%d -->", i++);
            for(int adiacentNodeId: v){
                printf(" %d", adiacentNodeId);
            }
            printf("\n");
        }
        infVec.resize(destinationNodes.size());
    }

    //--------------------------------------------------------------------------------
    bool conex(){
        for(Inf& inf: infVec){inf = Inf{};}
        std::queue<int> queue;
        infVec[0].visited = true;
        queue.push(0);
        while(!queue.empty()){
            int currId = queue.front();
            queue.pop();
            for(int id: destinationNodes[currId]){
                if(infVec[id].visited == false){
                    infVec[id].visited = true;
                    queue.push(id);
                }
            }
        }
        for(Inf& inf: infVec){
            if(inf.visited == false){
                return false;
            }
        }
        return true;
    }

    //--------------------------------------------------------------------------------
    int excentricitate(int nodeId){
        for(Inf& inf: infVec){inf = Inf{};}
        int maxDist = 0;
        std::queue<int> queue;
        infVec[nodeId].visited = true;
        infVec[nodeId].dist = 0;
        queue.push(nodeId);
        while(!queue.empty()){
            int currId = queue.front();
            queue.pop();
            if(maxDist < infVec[currId].dist){
                maxDist = infVec[currId].dist;
            }
            for(int id: destinationNodes[currId]){
                if(infVec[id].visited == false){
                    infVec[id].visited = true;
                    infVec[id].dist = maxDist+1;
                    queue.push(id);
                }
            }
        }
        return maxDist;
    }

    //--------------------------------------------------------------------------------
    int diameterAndRadius(){
    }
};

//================================================================================
int main(){
    GrafNeorientat g;
    g.read("2_GrafNeorientat.test");
    g.print();

    if(g.conex() == false){
        printf("ERR graful nu este conex!");
        return 1;
    }

    int diam = 0;
    int raza = INT_MAX;
    for(int i=0; i<g.destinationNodes.size(); ++i){
        int e = g.excentricitate(i);
        printf("excentricitate(%d) = %d\n", i, e);
        if(diam < e)
            diam = e;
        if(raza > e)
            raza = e;
    }
    printf("diam = %d\n", diam);
    printf("raza = %d\n", raza);

    return 0;
}