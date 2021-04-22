#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>

//--------------------------------------------------------------------------------
struct GrafOrientat{
    std::vector<std::vector<int>> destinationNodes;
    std::vector<std::vector<int>> sourceNodes;

    struct Inf{
        bool visited = false;
        int  prev    = 0;
        bool inverse = false;
    };
    std::vector<Inf> infVec;

    //--------------------------------------------------------------------------------
    void read(const char* filename){
        std::ifstream f(filename);
        int nr_noduri;
        f >> nr_noduri;
        destinationNodes.resize(nr_noduri);
        for(int i=0; i<nr_noduri; i++){
            int nodeId = 0;
            f >> nodeId;
            int nr_nod_adiac = 0;
            f >> nr_nod_adiac;
            for(int j=0; j<nr_nod_adiac; j++){
                int adiacentNodeId = 0;
                f >> adiacentNodeId;
                destinationNodes[i].push_back(adiacentNodeId);
            }
        }
        sourceNodes.resize(destinationNodes.size());
        int i = 0;
        for(int i=0; i< destinationNodes.size(); ++i){
            for(int id: destinationNodes[i]){
                sourceNodes[id].push_back(i);
            }
        }
        infVec.resize(destinationNodes.size());
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
    }

    //--------------------------------------------------------------------------------
    bool process(int id0, int currentNodeId, int id1, const std::vector<int>& vect, std::queue<int>& queue, bool inverse){
        for(int id: vect){
            if(infVec[id].visited == false){
                infVec[id].prev = currentNodeId;
                infVec[id].visited = true;
                infVec[id].inverse = inverse;
                if(id == id1){//destination node reached
                    std::stack<int> stack;
                    do{
                        stack.push(id);
                        id = infVec[id].prev;
                    }while(id != id0);
                    //stack.push(id0);

                    int prevId = id0;
                    while(stack.empty() == false){
                        int nodeId = stack.top();
                        if(infVec[nodeId].inverse){
                            printf(" (%d,%d)", nodeId, prevId);
                        }else{
                            printf(" (%d,%d)", prevId, nodeId);
                        }
                        prevId = nodeId;
                        stack.pop();
                    }
                    printf("\n");
                    return true;
                }
                queue.push(id);
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------------
    bool drum(int id0, int id1){
        for(Inf& inf: infVec){inf = Inf{};}
        std::queue<int> queue;
        infVec[id0].visited = true;
        queue.push(id0);
        while(!queue.empty()){
            int nodeId = queue.front();
            queue.pop();
            if(process(id0, nodeId, id1, destinationNodes[nodeId], queue, false)){
                return true;
            }
        }
        printf(" NONE\n");
        return false;
    }

    //--------------------------------------------------------------------------------
    bool lant(int id0, int id1){
        std::vector<std::vector<int>> sourceNodes(destinationNodes.size());
        int i = 0;
        for(int i=0; i< destinationNodes.size(); ++i){
            for(int id: destinationNodes[i]){
                sourceNodes[id].push_back(i);
            }
        }

        for(Inf& inf: infVec){inf = Inf{};}
        std::queue<int> queue;
        infVec[id0].visited = true;
        queue.push(id0);
        while(!queue.empty()){
            int nodeId = queue.front();
            queue.pop();
            if(process(id0, nodeId, id1, destinationNodes[nodeId], queue, false)){
                return true;
            }
            if(process(id0, nodeId, id1, sourceNodes[nodeId], queue, true)){
                return true;
            }
        }
        printf(" NONE\n");
        return false;
    }

    //--------------------------------------------------------------------------------
    void conex(int nodeId, int connexId){
        std::queue<int> queue;
        infVec[nodeId].visited = true;
        queue.push(nodeId);
        while(!queue.empty()){
            int currId = queue.front();
            queue.pop();
            for(int id: destinationNodes[currId]){
                if(infVec[id].visited == false){
                    printf(" %d", id);
                    infVec[id].visited = true;
                    queue.push(id);
                }
            }
        }
    }

    //--------------------------------------------------------------------------------
    void conexe(){
        int connexId = 0;
        for(Inf& inf: infVec){inf = Inf{};}
        for(int i=0; i<infVec.size(); ++i){
            if(infVec[i].visited == false){//not processed yet
                ++connexId;
                printf("componenta conexa %d: %d", connexId, i);
                conex(i, connexId);
                printf("\n");
            }
        }
        printf("total componente conexe: %d\n", connexId);
    }
};

//================================================================================
int main(){
    GrafOrientat g;
    g.read("1_GrafOrientat.test");
    g.print();

    printf("drum(0, 4):");
    g.drum(0, 4);

    printf("drum(5, 1):");
    g.drum(5, 1);

    printf("drum(4, 0):");
    g.drum(4, 0);

    printf("drum(4, 1):");
    g.drum(4, 1);

    printf("lant(0, 4):");
    g.lant(0, 4);

    printf("lant(4, 0):");
    g.lant(4, 0);

    printf("lant(4, 1):");
    g.lant(4, 1);

    printf("lant(5, 1):");
    g.lant(5, 1);

    g.conexe();
    return 0;
}