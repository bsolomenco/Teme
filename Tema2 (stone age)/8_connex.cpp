#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(const char* buf) {
    std::vector<int> vec;
    const auto* end = buf + strlen(buf);
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
    for (auto line : matrix) {
        for (auto elem : line) {
            fprintf(file, "%2d ", elem);
        }
        fprintf(file, "\n");
    }
}

//================================================================================
struct Connex {
    struct Cell {
        int row = 0;
        int col = 0;
    };
    struct Obj {
        int id = 0;
        int top = 0;
        int left = 0;
        int bottom = 0;
        int right = 0;
        int area = 0;//area (number of cells)
    };

    std::vector<Obj> objects;

    Connex(Matrix& matrix);
    void run();
    void visit(int id, int row, int col, std::queue<Cell>& queue);
    int maxAreaObjIndex() {
        return _maxAreaObjIndex;
    }
//data members
    Matrix& _matrix;
    int     _objId = 1;
    Obj     _obj;
    int     _maxAreaObjIndex = 0;
};

//--------------------------------------------------------------------------------
Connex::Connex(Matrix& matrix)
    : _matrix(matrix)
{}

//--------------------------------------------------------------------------------
void Connex::run() {
    int rows = (int)_matrix.size();
    int cols = (int)_matrix[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (_matrix[i][j] == 1) {//cell belongs to an object
                _matrix[i][j] = ++_objId;//assign an new unique identifier (sequence number > 1)
                _obj = { _objId, i, j, i, j, 1 };
                std::queue<Cell> cellsToVisit;
                cellsToVisit.push({ i,j });
                while (!cellsToVisit.empty()) {
                    Connex::Cell& cell = cellsToVisit.front();
                    cellsToVisit.pop();
                    //check all cells around current cell
                    visit(_objId, cell.row-1, cell.col-1, cellsToVisit);
                    visit(_objId, cell.row-1, cell.col  , cellsToVisit);
                    visit(_objId, cell.row-1, cell.col+1, cellsToVisit);
                    visit(_objId, cell.row  , cell.col-1, cellsToVisit);
                    visit(_objId, cell.row  , cell.col+1, cellsToVisit);
                    visit(_objId, cell.row+1, cell.col-1, cellsToVisit);
                    visit(_objId, cell.row+1, cell.col  , cellsToVisit);
                    visit(_objId, cell.row+1, cell.col+1, cellsToVisit);
                }
                objects.push_back(_obj);
                if (objects[_maxAreaObjIndex].area < _obj.area) {
                    _maxAreaObjIndex = _objId - 2;
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------
void Connex::visit(int id, int row, int col, std::queue<Cell>& queue) {
    int rows = (int)_matrix.size();
    int cols = (int)_matrix[0].size();
    if (0 <= row && row < rows && 0 <= col && col < cols && _matrix[row][col] == 1) {//valid, marked as original object
        _matrix[row][col] = id;//assign current unique identifier (sequence number > 1)
        _obj.area++;
        if (row < _obj.top)
            _obj.top = row;
        if (row > _obj.bottom)
            _obj.bottom = row;
        if (col < _obj.left)
            _obj.left = col;
        if (col > _obj.right)
            _obj.right = col;
        queue.push({ row,col });
    }
}
//--------------------------------------------------------------------------------
void writeSubmatrixToFile(const Matrix& matrix, const Connex::Obj& obj, FILE* outputFile){
    int id = obj.id;
    for(int i=obj.top; i<=obj.bottom; ++i){
        for(int j=obj.left; j<=obj.right; ++j){
            fprintf(outputFile, "%d ", matrix[i][j]==obj.id ? id : 0);
        }
        fprintf(outputFile, "\n");
    }
}

//================================================================================
int main( int argc, char** argv){

    FILE* inputFile = fopen("8_connex.test","rt");
    auto matrix = readMatrixFromFile(inputFile);
    if(matrix.size() == 0){
        printf("ERR empty or wrong input\n");
        return 2;
    }
    printf("input matrix [%zu x %zu]:\n", matrix.size(), matrix[0].size());
    writeSubmatrixToFile(matrix, Connex::Obj{1, 0, 0, (int)matrix.size()-1, (int)matrix[0].size()-1}, stdout);

    Connex connex(matrix);
    connex.run();
    printf("connex objects : %zu\n", connex.objects.size());
    auto maxArea = connex.objects[connex.maxAreaObjIndex()].area;
    printf("max object area: %d\n", connex.objects[connex.maxAreaObjIndex()].area);

    int index = 0;
    char buf[256] = {};
    for(Connex::Obj &obj: connex.objects){
        printf("connex object index=%d id=%d area=%d%s size=[%d x %d]\n", index, obj.id, obj.area, obj.area==maxArea?"=MAX":"", obj.bottom-obj.top+1, obj.right-obj.left+1);
        writeSubmatrixToFile(matrix, obj, stdout);
        sprintf(buf, "ConnexObject%d.out", index);
        FILE* outputFile = fopen(buf, "wt");
        if(outputFile){
            writeSubmatrixToFile(matrix, obj, outputFile);
            fclose(outputFile);
        }
        ++index;
    }
    fclose(inputFile);
    return 0;
}