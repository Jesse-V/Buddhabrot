
#ifndef MAIN
#define MAIN

#include <algorithm>
#include <vector>
#include <string>

typedef std::vector<std::vector<unsigned int>> Matrix2D;

Matrix2D generateBuddhabrotHistogram();
void initializeMatrix(Matrix2D& matrix);
void fillMatrixWithBuddhabrot(Matrix2D& matrix);
void updateCounter(Matrix2D& matrix, float x, float y);

void printMatrix(Matrix2D& matrix);
void writeMatrixToPPM(Matrix2D& matrix, std::string filename);
std::mt19937 getMersenneTwister();

#endif
