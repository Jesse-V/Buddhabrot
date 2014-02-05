
#ifndef MAIN
#define MAIN

#include <algorithm>
#include <vector>
#include <string>

typedef std::vector<std::vector<unsigned long>> Matrix2D;

void writeMatrixToPPM(Matrix2D& matrix, std::string filename);

#endif
