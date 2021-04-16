#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <stack>
#include <sstream>
#include <queue>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(const char* buf) {
    std::vector<int> vec;
    const char* end = buf + strlen(buf);
    int value = 0;
    std::istringstream iss(buf);
    while(iss >> value){
        vec.push_back(value);
    }
    return vec;
}

//--------------------------------------------------------------------------------
Matrix readMatrixFromFile(FILE* file) {
    char buf[1024] = {};
    size_t columns = 0;
    std::vector<std::vector<int>> matrix;
    for (int line = 0; fgets(buf, sizeof(buf), file); ++line) {
        if (buf[0] == '\n') {//stop at 1st empty line
            break;
        }
        if (buf[0] == '#') {//skip comments
            continue;
        }
        std::vector<int> matrixLine = parseLineOfNumbers(buf);
        if (columns == 0) {
            columns = matrixLine.size();
            matrix.push_back(matrixLine);
            continue;//next line
        }
        if (matrixLine.size() != columns) {
            printf("ERR invalid number of elements on line %d: expect %zu found %zu\n", line, columns, matrixLine.size());
            matrix.clear();
            break;
        }
        matrix.push_back(matrixLine);
    }
    return matrix;
}

//--------------------------------------------------------------------------------
void writeMatrixToFile(const Matrix& matrix, FILE* file) {
    for (const std::vector<int>& line : matrix) {
        for (int elem : line) {
            fprintf(file, "%2d ", elem);
        }
        fprintf(file, "\n");
    }
}

//================================================================================
struct Maze {
    struct Cell {
        int row = 0;
        int col = 0;
    };

    enum CellType {
        WALL = -1,
        ROAD,
        SOLUTION,
    };

    Maze(Matrix& matrix);
    void run(int row0, int col0, int row1, int col1);
    int minimalSolution() {
        return _minimalSolution;
    }

//data members
    Matrix& _matrix;
    int     _minimalSolution = 0;

    void visitRoadCell(int row, int col, int offsetRow, int offsetCol, int id, std::queue<Cell>& queue);
    void visitSolutionCell(int row, int col, int dist, std::queue<Cell>& queue);
};

//--------------------------------------------------------------------------------
Maze::Maze(Matrix& matrix)
    : _matrix(matrix)
{}

//--------------------------------------------------------------------------------
#if 0
breadth - first search starting from(row1, col1)
mark each road cell with the distance to(row1, col1)
if (row0, col0) found then the solution are :
any sequence N, N - 1, ..., 1 starting from(row0, col0)
#endif
void Maze::run(int row0, int col0, int row1, int col1) {
    if (_matrix[row0][col0] || _matrix[row1][col1]) {
        return;
    }
    _minimalSolution = 0;
    std::queue<Cell> cellsToVisit;
    _matrix[row1][col1] = CellType::SOLUTION;//distance from (row1,col1) to (row1,col1)
    cellsToVisit.push({ row1,col1 });
    while (!cellsToVisit.empty()) {
        Maze::Cell cell = cellsToVisit.front();
        cellsToVisit.pop();
        if (cell.row == row0 && cell.col == col0) {
            break;
        }
        int id = 1 + _matrix[cell.row][cell.col];//distance from (row,col) to (row1,col1)
        //check all cells around current cell: up, down, left, right
        visitRoadCell(cell.row, cell.col, -1,  0, id, cellsToVisit);//up
        visitRoadCell(cell.row, cell.col,  0, -1, id, cellsToVisit);//left
        visitRoadCell(cell.row, cell.col,  0, +1, id, cellsToVisit);//right
        visitRoadCell(cell.row, cell.col, +1,  0, id, cellsToVisit);//down
    }
    if (_matrix[row0][col0]) {//solution found
        _minimalSolution = _matrix[row0][col0];
        std::queue<Cell> queue;
    }
}

//--------------------------------------------------------------------------------
void Maze::visitRoadCell(int row, int col, int rowOffset, int colOffset, int id, std::queue<Cell>& queue) {
    int rows = (int)(_matrix.size());
    int cols = (int)(_matrix[0].size());
    row += rowOffset;
    col += colOffset;
    if (0 <= row && row < rows && 0 <= col && col < cols && _matrix[row][col] == 0) {//valid, marked as road cell
        _matrix[row][col] = id;//distance to cheese
        queue.push({ row, col });
    }
}

//--------------------------------------------------------------------------------
void Maze::visitSolutionCell(int row, int col, int dist, std::queue<Cell>& queue) {
    int rows = (int)(_matrix.size());
    int cols = (int)(_matrix[0].size());
    if (0 <= row && row < rows && 0 <= col && col < cols && _matrix[row][col] == dist) {
        queue.push({ row, col });
    }
}

//================================================================================
int main( int argc, char** argv){

    FILE* file = fopen("9_maze.test", "rt");
    Matrix matrix = readMatrixFromFile(file);
    if(matrix.size() == 0){
        printf("ERR empty or wrong input\n");
        return 2;
    }
    printf("input matrix [%zu x %zu]:\n", matrix.size(), matrix[0].size());
    writeMatrixToFile(matrix, stdout);

    int row0 = 0;
    int col0 = 0;
    int row1 = 0;
    int col1 = 0;
    {//read coordinates
        char buf[1024] = {};
        for(int line=0; fgets(buf, sizeof(buf), file); ++line){
            if(buf[0]=='\n' || buf[0]=='#'){//skip empty lines and comments
                continue;
            }
            break;
        }
        int count = sscanf(buf, "%d %d %d %d", &row0, &col0, &row1, &col1);
        if(count != 4){
            printf("ERR wrong coordinates: expect 4 found %d\n", count);
            return 2;
        }
        printf("row0=%d col0=%d row1=%d col1=%d\n", row0, col0, row1, col1);
        if(matrix[row0][col0] || matrix[row1][col1]){
            printf("ERR wrong coordinates: not on road cells\n");
            return 3;
        }
    }

    Maze maze(matrix);
    maze.run(row0, col0, row1, col1);
    if(!maze.minimalSolution()){
        printf("no solution found!\n");
        return 3;
    }
    printf("minimal solution(s) with distance=%d: any sequence %d..1 starting from (row0=%d col0=%d):\n", maze.minimalSolution(), maze.minimalSolution(), row0, col0);
    writeMatrixToFile(matrix, stdout);
    printf("one possible minimal solution:\n");
    int rows = (int)matrix.size();
    int cols = (int)matrix[0].size();
    int row = row0;
    int col = col0;
    int dist = 0;
    while (1) {
        dist = matrix[row][col];
        printf("(%d,%d) dist=%d\n", row, col, dist);
        matrix[row][col] = 0;
        if (row == row1 && col == col1) {
            break;
        }
        --dist;//pick a neighbor which is one step closer to destination
        if (0 < row && matrix[row - 1][col] == dist) {//move up
            --row;
            continue;
        }
        if (col + 1 < cols && matrix[row][col + 1] == dist) {//move right
            ++col;
            continue;
        }
        if (row + 1 < rows && matrix[row + 1][col] == dist) {//move down
            ++row;
            continue;
        }
        if (0 < col && matrix[row][col - 1] == dist) {//move left
            --col;
            continue;
        }
    }
    fclose(file);
    return 0;
}