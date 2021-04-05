#include "C_isatty.h"
#include "Matrix.hpp"
#include "Maze.hpp"
#include <charconv>
#include <cstdio>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

//--------------------------------------------------------------------------------
void showHelp(){
    printf(
        "usage : Maze < inputFile\n"
        "input file syntax: matrix with space separated integers {0,-1}, one matrix line per file line, followed by coordinates (row0 col0 row1 col1), like:\n"
        "    0  0  0 -1\n"
        "    0 -1  0  0\n"
        "\n"
        "    0 0 1 2\n"
        "number of columns is given by the number of elements in the 1st line\n"
    );
}

//================================================================================
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    auto redirectedStdin = !isatty(STDIN_FILENO);
    if(!redirectedStdin){//console input not redirected => display help message
        showHelp();
        return 1;
    }
    auto matrix = readMatrixFromFile(stdin);
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
        for(int line=0; fgets(buf, sizeof(buf), stdin); ++line){
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
    printf("minimal solution(s) (distance=%d) marked with 1:\n", maze.minimalSolution());
    writeMatrixToFile(matrix, stdout);
    printf("one possible minimal solution:\n");
    int rows = (int)matrix.size();
    int cols = (int)matrix[0].size();
    auto row = row0;
    auto col = col0;
    while(1){
        printf("(%d,%d)\n", row, col);
        matrix[row][col] = 0;
        if(row==row1 && col==col1){
            break;
        }
        if(0<row && matrix[row-1][col]==1){//move up
            --row;
            continue;
        }
        if(col+1<cols && matrix[row][col+1]==1){//move right
            ++col;
            continue;
        }
        if(row+1<rows && matrix[row+1][col]==1){//move down
            ++row;
            continue;
        }
        if(0<col && matrix[row][col-1]==1){//move left
            --col;
            continue;
        }
    }
    return 0;
}