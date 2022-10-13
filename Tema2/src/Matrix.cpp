#include "Matrix.hpp"
#include <charconv>
#include <cstdio>
#include <vector>
#include <system_error>


//--------------------------------------------------------------------------------
std::vector<int> parseLineOfNumbers(const char* buf){
    std::vector<int> vec;
    const auto* end = buf+strlen(buf);
    int value = 0;
    for(auto result=std::from_chars(buf, end, value); result.ec==std::errc(); result=std::from_chars(result.ptr, end, value)){
        vec.push_back(value);
        while(iswspace(*result.ptr)){
            result.ptr++;
        }
    }
    return vec;
}

//--------------------------------------------------------------------------------
Matrix readMatrixFromFile(FILE* file){
    char buf[1024] = {};
    size_t columns = 0;
    std::vector<std::vector<int>> matrix;
    for(int line=0; fgets(buf, sizeof(buf), file); ++line){
        if(buf[0] == '\n'){//stop at 1st empty line
            break;
        }
        if(buf[0] == '#'){//skip comments
            continue;
        }
        std::vector<int> matrixLine = parseLineOfNumbers(buf);
        if(columns == 0){
            columns = matrixLine.size();
            matrix.push_back(matrixLine);
            continue;//next line
        }
        if(matrixLine.size() != columns){
            printf("ERR invalid number of elements on line %d: expect %zu found %zu\n", line, columns, matrixLine.size());
            matrix.clear();
            break;
        }
        matrix.push_back(matrixLine);
    }
    return matrix;
}

//--------------------------------------------------------------------------------
void writeMatrixToFile(const Matrix& matrix, FILE* file){
    for(auto line: matrix){
        for(auto elem: line){
            fprintf(file, "%2d ", elem);
        }
        fprintf(file, "\n");
    }
}