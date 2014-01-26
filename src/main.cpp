
#include "main.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

const int MAX_DEPTH = 300;
const int MAX_ITERATIONS = 10000;
const std::size_t IMAGE_SIZE = 10;


int main(int argc, char** argv)
{
    Matrix2D matrix;
    initializeMatrix(matrix);

    fillMatrixWithBuddhabrot(matrix);

    printMatrix(matrix);
    writeMatrixToPPM(matrix);

    return EXIT_SUCCESS;
    //return EXIT_FAILURE;
}



void initializeMatrix(Matrix2D& matrix)
{
    for (std::size_t j = 0; j < IMAGE_SIZE; j++)
        matrix.push_back(std::vector<unsigned int>(IMAGE_SIZE, 0));
}



void fillMatrixWithBuddhabrot(Matrix2D& matrix)
{
    /*double ptYSq = pt.y * pt.y;
    double xOff = pt.x - 0.25;
    double q = Math.pow(xOff, 2) + ptYSq;
    if (q * (q + xOff) < ptYSq / 4)
            return Color.BLACK; //http://en.wikipedia.org/wiki/Mandelbrot_fractal#Optimizations
*/
    static std::mt19937 mersenneTwister;
    static std::uniform_real_distribution<float> randomFloat(-2, 2);

    std::vector<std::pair<float, float>> points;
    points.resize(MAX_DEPTH);

    for (int j = 0; j < MAX_ITERATIONS; j++)
    {
        float ptX = randomFloat(mersenneTwister);
        float ptY = randomFloat(mersenneTwister);

        float x = 0, xSq = 0, y = 0, ySq = 0;
        int iterations;
        for (iterations = 0; iterations < MAX_DEPTH && (xSq + ySq <= 64); iterations++)
        {
            y = 2 * x * y + ptY;
            x = xSq - ySq + ptX;
            xSq = x * x;
            ySq = y * y;
        }

        if (iterations == MAX_DEPTH) //given point has not escaped
            for (const auto &point : points)
                updateCounter(matrix, point.first, point.second);
    }
}



void updateCounter(Matrix2D& matrix, float fractalX, float fractalY)
{

}



void printMatrix(Matrix2D& matrix)
{
    for (const auto &row : matrix)
    {
        for (const auto &cell : row)
            std::cout << cell << " ";
        std::cout << std::endl;
    }
}



void writeMatrixToPPM(Matrix2D& matrix)
{
    std::ofstream ppm;
    ppm.open("image.ppm", std::ofstream::out);

    ppm << "P3 2 2 255" << std::endl;
    ppm << "255 0 0 0 255 0" << std::endl;
    ppm << "0 0 255 0 255 255" << std::endl;

    ppm.close();
}



unsigned int getMax(Matrix2D& matrix)
{
    unsigned int max = 0;
    for (const auto &row : matrix)
        for (const auto &cell : row)
            if (cell > max)
                max = cell;
    return max;
}
