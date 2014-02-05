
#ifndef MAIN
#define MAIN

#include <algorithm>
#include <vector>
#include <string>

typedef std::vector<std::vector<unsigned long>> Matrix2D;

Matrix2D generateBuddhabrotHistogram();
void initializeMatrix(Matrix2D& matrix);
void fillMatrixWithBuddhabrot(Matrix2D& matrix);
void updateCounter(Matrix2D& matrix, float x, float y);

void printMatrix(Matrix2D& matrix);
void writeMatrix(Matrix2D& matrix, std::string filename);
std::mt19937 getMersenneTwister();

#endif
