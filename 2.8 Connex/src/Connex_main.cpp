#include "C_isatty.h"
#include "Connex.hpp"
#include "Matrix.hpp"
#include <charconv>
#include <cstdio>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

//--------------------------------------------------------------------------------
void showHelp(){
    printf(
        "usage : Connex < inputFile\n"
        "input file syntax: matrix with space separated integers {0,1}, one matrix line per file line, like:\n"
        "    0 0 0\n"
        "    0 1 0\n"
        "number of columns is given by the number of elements in the 1st line\n"
        "last matrix line is last non-empty line from input\n"
    );
}

//--------------------------------------------------------------------------------
void writeSubmatrixToFile(const Matrix& matrix, const Connex::Obj& obj, FILE* file){
    //check if object's coordinates is included in matrix
    //...
#if 0 //print "1" for each object's cell
    int id = 1;
#else //print object's ID for each of its cell
    int id = obj.id;
#endif

    for(int i=obj.top; i<=obj.bottom; ++i){
        for(int j=obj.left; j<=obj.right; ++j){
            fprintf(file, "%d ", matrix[i][j]==obj.id ? id : 0);
        }
        fprintf(file, "\n");
    }
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
    writeSubmatrixToFile(matrix, Connex::Obj{1, 0, 0, (int)matrix.size()-1, (int)matrix[0].size()-1}, stdout);

    Connex connex(matrix);
    connex.run();
    printf("connex objects : %zu\n", connex.objects.size());
    auto maxArea = connex.objects[connex.maxAreaObjIndex()].area;
    printf("max object area: index=%d area=%d\n", connex.maxAreaObjIndex(), connex.objects[connex.maxAreaObjIndex()].area);

    int index = 0;
    char buf[256] = {};
    for(auto obj: connex.objects){
        printf("connex object index=%d id=%d area=%d%s size=[%d x %d]\n", index, obj.id, obj.area, obj.area==maxArea?"=MAX":"", obj.bottom-obj.top+1, obj.right-obj.left+1);
        writeSubmatrixToFile(matrix, obj, stdout);
        sprintf(buf, "ConnexObject%d.out", index);
        auto file = fopen(buf, "wt");
        if(file){
            writeSubmatrixToFile(matrix, obj, file);
            fclose(file);
        }
        ++index;
    }

    return 0;
}