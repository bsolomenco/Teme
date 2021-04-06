#pragma once
#include "Matrix.hpp"
#include <queue>
#include <vector>


class Connex{
public:
    struct Cell{
        int row = 0;
        int col = 0;
    };
    struct Obj{
        int id      = 0;
        int top     = 0;
        int left    = 0;
        int bottom  = 0;
        int right   = 0;
        int area    = 0;//area (number of cells)
    };

    std::vector<Obj> objects;

    Connex(Matrix& matrix);
    void run();
    void visit(int id, int row, int col, std::queue<Cell>& queue);
    int maxAreaObjIndex(){
        return _maxAreaObjIndex;
    }
private:
    Matrix& _matrix;
    int     _objId = 1;
    Obj     _obj;
    int     _maxAreaObjIndex = 0;
};