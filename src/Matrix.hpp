#pragma once
#include "Matrix.hpp"
#include <vector>

using Matrix = std::vector<std::vector<int>>;

Matrix readMatrixFromFile(FILE* file);
void writeMatrixToFile(const Matrix& matrix, FILE* file);