
#ifndef MAIN
#define MAIN

#include <vector>

typedef std::vector<std::vector<unsigned int>> Matrix2D;

void initializeMatrix(Matrix2D& matrix);

void fillMatrixWithBuddhabrot(Matrix2D& matrix);
void updateCounter(Matrix2D& matrix, float x, float y);

void printMatrix(Matrix2D& matrix);
void writeMatrixToPPM(Matrix2D& matrix);
unsigned int getMax(Matrix2D& matrix);

#endif
