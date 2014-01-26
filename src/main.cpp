
#include "main.hpp"
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>

const int MAX_DEPTH = 20;
const int MAX_ITERATIONS = 50000;
const int IMAGE_SIZE = 1024;


int main(int argc, char** argv)
{
    Matrix2D matrix;
    initializeMatrix(matrix);

    fillMatrixWithBuddhabrot(matrix);

    //printMatrix(matrix);
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
    static std::mt19937 mersenneTwister;
    static std::uniform_real_distribution<float> randomFloat(-2, 2);

    std::vector<std::complex<float>> points;
    points.resize(MAX_DEPTH);

    for (int j = 0; j < MAX_ITERATIONS; j++)
    {
        float ptX = randomFloat(mersenneTwister);
        float ptY = randomFloat(mersenneTwister);

        std::complex<float> c(ptX, ptY);
        std::complex<float> z(0, 0);
        int iterations;
        for (iterations = 0; iterations < MAX_DEPTH && norm(z) < 4; iterations++)
        {
            z = z * z + c;
            points.push_back(z);
        }

        if (iterations == MAX_DEPTH) //given point has not escaped
            //updateCounter(matrix, ptX, ptY);
            for (const auto &point : points)
                updateCounter(matrix, point.real(), point.imag());
    }
}



void updateCounter(Matrix2D& matrix, float fractalX, float fractalY)
{
    int x = (int)((fractalX + 2) / 4 * IMAGE_SIZE);
    int y = (int)((fractalY + 2) / 4 * IMAGE_SIZE);

    if (x >= 0 && x < IMAGE_SIZE && y >= 0 && y < IMAGE_SIZE)
        matrix[(std::size_t)x][(std::size_t)y]++;
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
    long sum = 0;
    for (const auto &row : matrix)
        for (const auto &cell : row)
            sum += cell;

    float max = 0;
    for (const auto &row : matrix)
        for (const auto &cell : row)
            if (cell / (float)sum > max)
                max = cell / (float)sum;

    std::ofstream ppm;
    ppm.open("image.ppm", std::ofstream::out);

    ppm << "P3 " << IMAGE_SIZE << " " << IMAGE_SIZE << " 255" << std::endl;
    for (const auto &row : matrix)
    {
        for (const auto &cell : row)
        {
            float scale = cell / (float)sum / max;
            int grayscale = (int)(scale * 255);
            ppm << grayscale << " " << grayscale << " " << grayscale << " ";
        }
        ppm << std::endl;
    }

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
