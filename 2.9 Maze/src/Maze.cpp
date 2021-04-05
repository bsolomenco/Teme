#include <Maze.hpp>

//--------------------------------------------------------------------------------
Maze::Maze(Matrix& matrix)
    : _matrix(matrix)
{}

//--------------------------------------------------------------------------------
#if 0
breadth-first search starting from (row1,col1)
    mark each road cell with the distance to (row1,col1)
    if (row0,col0) found then the solution are:
        any sequence N, N-1, ..., 1 starting from (row0,col0)
#endif
void Maze::run(int row0, int col0, int row1, int col1){
    if(_matrix[row0][col0] || _matrix[row1][col1]){
        return;
    }
    _minimalSolution = 0;
    auto rows = _matrix.size();
    auto cols = _matrix[0].size();
    std::queue<Cell> cellsToVisit;
    _matrix[row1][col1] = CellType::SOLUTION;//distance from (row1,col1) to (row1,col1)
    cellsToVisit.push({row1,col1});
    while(!cellsToVisit.empty()){
        auto [row,col] = cellsToVisit.front();
        cellsToVisit.pop();
        if(row==row0 && col==col0){
            break;
        }
        int id = 1+_matrix[row][col];//distance from (row,col) to (row1,col1)
        //check all cells around current cell: up, down, left, right
        visitRoadCell(row, col, -1,  0, id, cellsToVisit);//up
        visitRoadCell(row, col,  0, -1, id, cellsToVisit);//left
        visitRoadCell(row, col,  0, +1, id, cellsToVisit);//right
        visitRoadCell(row, col, +1,  0, id, cellsToVisit);//down
    }
    if(_matrix[row0][col0]){//solution found
        _minimalSolution = _matrix[row0][col0];
        std::queue<Cell> queue;
        queue.push({row0,col0});
        while(!queue.empty()){
            auto [row,col] = queue.front();
            queue.pop();
            if(row==row1 && col==col1){
                continue;
            }
            auto dist = _matrix[row][col]-1;
            _matrix[row][col] = CellType::SOLUTION;
            visitSolutionCell(row-1, col  , dist, queue);
            visitSolutionCell(row  , col+1, dist, queue);
            visitSolutionCell(row+1, col  , dist, queue);
            visitSolutionCell(row  , col-1, dist, queue);
        }
    }
    //clean unused roads in matrix
    for(auto& line: _matrix){
        for(auto& elem: line){
            if(elem!=CellType::WALL && elem!=CellType::SOLUTION){
                elem = 0;
            }
        }
    }
}

//--------------------------------------------------------------------------------
void Maze::visitRoadCell(int row, int col, int rowOffset, int colOffset, int id, std::queue<Cell>& queue){
    auto rows = _matrix.size();
    auto cols = _matrix[0].size();
    row += rowOffset;
    col += colOffset;
    if(0<=row && row<rows && 0<=col && col<cols && _matrix[row][col]==0){//valid, marked as road cell
        _matrix[row][col] = id;//distance to cheese
        queue.push({row, col});
    }
}

//--------------------------------------------------------------------------------
void Maze::visitSolutionCell(int row, int col, int dist, std::queue<Cell>& queue){
    auto rows = _matrix.size();
    auto cols = _matrix[0].size();
    if(0<=row && row<rows && 0<=col && col<cols && _matrix[row][col]==dist){
        queue.push({row, col});
    }
}