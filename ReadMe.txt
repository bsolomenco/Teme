Prerequisites
    CMake: https://cmake.org/download/

Repo
...

Build
    Linux/gcc   : rm -rf .tmp   ; cmake -B .tmp && cmake --build .tmp --target install --config Release
    Windows/VS  : rd /Q /S .tmp & cmake -B .tmp && cmake --build .tmp --target install --config Release


Stack (list based, type=int)

Queue (list based, type=int)

Parens
- obvious rule: matching pairs: () [] {}
- unclear rule: nesting rank ("sirul ([]) nu este corect") could be:
    () < [] < {}
    () <= [] <= {}

Conex
struct Obj{
    int top     = 0;
    int left    = 0;
    int bottom  = 0;
    int right   = 0;
    int area    = 0;
};
std::vector<Obj> objects;
- algorithm:
    - set
        int objId = 1;
        int maxAreaObjIndex = 0;
        int top=0, bottom=0, left=0, right=0;
    - iterate matrix cells (left->right and top->bottom) and for each cell i,j:
        if(m[i][j] == 1){
            m[i][j] = ++objId;
            int area = 1;
            top=i; bottom=i; left=j; right=j;
            stack.push({i,j});
            while(!stack.empty())
                [i,j] = stack.top();
                stack.pop()
                setIdForConexCell(i-1, j-1);
                setIdForConexCell(i-1, j  );
                setIdForConexCell(i-1, j+1);
                setIdForConexCell(i  , j-1);
                setIdForConexCell(i  , j+1);
                setIdForConexCell(i+1, j-1);
                setIdForConexCell(i+1, j  );
                setIdForConexCell(i+1, j+1);
            }
            objects.push_back({top, left, bottom, right, area});
            if(objects[maxAreaObjIndex].area < area){
                maxAreaObjIndex = objId-2;
            }
        }

    - setIdForConexCell(i, j)
            if(0<=i && i<H && 0<=j && j<W && m[i][j]==1){//valid and marked as object
                m[i][j] = objId;
                ++area;
                if(i < top){top=i;}
                if(i > bottom){bottom=i;}
                if(j < left){left=j;}
                if(j > right){right=j;}
                stack.push({i,j});
            }
    - objectCount = objId-1;//objects.size()
