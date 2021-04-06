#pragma once
#include "Matrix.hpp"
#include <queue>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

class Maze{
public:
    struct Cell{
        int row = 0;
        int col = 0;
    };

    enum CellType{
        WALL = -1,
        ROAD    ,
        SOLUTION,
    };

    Maze(Matrix& matrix);
    void run(int row0, int col0, int row1, int col1);
    int minimalSolution(){
        return _minimalSolution;
    }
private:
    Matrix& _matrix;
    int     _minimalSolution = 0;

    void visitRoadCell(int row, int col, int offsetRow, int offsetCol, int id, std::queue<Cell>& queue);
    void visitSolutionCell(int row, int col, int dist, std::queue<Cell>& queue);
};