#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>

typedef std::vector<std::vector<int>> Matrix;

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
    Matrix matrix;
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
void writeMatrixToFile(const Matrix& matrix, FILE* file){
    for(const std::vector<int>& line : matrix){
        for(int elem : line){
            fprintf(file, "%2d ", elem);
        }
        fprintf(file, "\n");
    }
}


//--------------------------------------------------------------------------------
struct Node{//tree node
    int row = 0;
    int col = 0;
    Node* parent = 0;
    //no need to manage kids (ONLY to free dynamic allocated memory... BUT this is not representative for this algorithm)
    //a vector<Node> could be used (pre-allocated for nxn) instead queue to avoid memory management but the "stars" are stack+queue+tree
};

//--------------------------------------------------------------------------------
void visit(Matrix& matrix, Node& parent, int rowOffset, int colOffset, std::queue<Node*>& queue) {
    int rows = (int)(matrix.size());
    int cols = (int)(matrix[0].size());
    int row = parent.row + rowOffset;
    int col = parent.col + colOffset;
    if(0<=row && row<rows && 0<=col && col<cols && matrix[row][col]>=0){//bounds check and not visited
        //matrix[row][col] *= -1;//mark as visited
        queue.push(new Node{row, col, &parent});
    }
}

//--------------------------------------------------------------------------------
#if 0
breadth-first search starting from (row0,col0) and moving up|down|left|right with [row][col] cells (if possible) until reaching  (row1,col1)
#endif
void findMinimalMoves(Matrix& matrix, int row0, int col0, int row1, int col1) {
    if(row0==row1 && col0==col1){
        printf("start == stop\n");
        return;
    }
    int rows = (int)(matrix.size());
    int cols = (int)(matrix[0].size());

    Node root{row0, col0};
    std::queue<Node*> queue;
    queue.push(&root);
    while(queue.empty() == false){
        Node* node = queue.front();
        queue.pop();
        if(node->row==row1 && node->col==col1){//solution found
            std::stack<Node*> stack;
            do{
                stack.push(node);
                node = node->parent;
            }while(node);
            printf("solution:\n");
            int minimalMovesNr = 0;
            while(stack.empty() == false){
                ++minimalMovesNr;
                Node* node = stack.top();
                stack.pop();
                printf("step %d: (%d,%d)\n", minimalMovesNr, node->row, node->col);
            }
            return;
        }
        int val = matrix[node->row][node->col];
        if(val == 0)
            continue;
        matrix[node->row][node->col] = -val;
        visit(matrix, *node, -val, 0   , queue);//up
        visit(matrix, *node, +val, 0   , queue);//down
        visit(matrix, *node,  0  , -val, queue);//left
        visit(matrix, *node,  0  , +val, queue);//right
    }
    printf("no solutiopn found!\n");
}


//================================================================================
int main(int argc, char**argv){
    FILE* file = fopen("3_JocNr.test", "rt");
    Matrix matrix = readMatrixFromFile(file);
    fclose(file);
    writeMatrixToFile(matrix, stdout);

    printf("please input row0, col0, ro1, col1: ");
    int row0=0, col0=0, row1=0, col1=0;
    scanf("%d%d%d%d", &row0, &col0, & row1, &col1);
    findMinimalMoves(matrix, row0, col0, row1, col1);
    return 0;
}