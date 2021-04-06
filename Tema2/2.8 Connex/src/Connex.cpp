#include "Connex.hpp"

//--------------------------------------------------------------------------------
Connex::Connex(Matrix& matrix)
    : _matrix(matrix)
{}

//--------------------------------------------------------------------------------
void Connex::run(){
    auto rows = static_cast<int>(_matrix.size());
    auto cols = static_cast<int>(_matrix[0].size());
    for(int i=0; i<rows; ++i){
        for(int j=0; j<cols; ++j){
            if(_matrix[i][j] == 1){//cell belongs to an object
                _matrix[i][j] = ++_objId;//assign an new unique identifier (sequence number > 1)
                _obj = {_objId, i, j, i, j, 1};
                std::queue<Cell> cellsToVisit;
                cellsToVisit.push({i,j});
                while(!cellsToVisit.empty()){
                    auto [row,col] = cellsToVisit.front();
                    cellsToVisit.pop();
                    //check all cells around current cell
                    visit(_objId, row-1, col-1, cellsToVisit);
                    visit(_objId, row-1, col  , cellsToVisit);
                    visit(_objId, row-1, col+1, cellsToVisit);
                    visit(_objId, row  , col-1, cellsToVisit);
                    visit(_objId, row  , col+1, cellsToVisit);
                    visit(_objId, row+1, col-1, cellsToVisit);
                    visit(_objId, row+1, col  , cellsToVisit);
                    visit(_objId, row+1, col+1, cellsToVisit);
                }
                objects.push_back(_obj);
                if(objects[_maxAreaObjIndex].area < _obj.area){
                    _maxAreaObjIndex = _objId-2;
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------
void Connex::visit(int id, int row, int col, std::queue<Cell>& queue){
    auto rows = static_cast<int>(_matrix.size());
    auto cols = static_cast<int>(_matrix[0].size());
    if(0<=row && row<rows && 0<=col && col<cols && _matrix[row][col]==1){//valid, marked as original object
        _matrix[row][col] = id;//assign current unique identifier (sequence number > 1)
        _obj.area++;
        if(row < _obj.top)
            _obj.top=row;
        if(row > _obj.bottom)
            _obj.bottom=row;
        if(col < _obj.left)
            _obj.left=col;
        if(col > _obj.right)
            _obj.right=col;
        queue.push({row,col});
    }
}