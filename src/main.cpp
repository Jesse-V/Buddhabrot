
#include "main.hpp"
#include <complex>
#include <fstream>
#include <future>
#include <iostream>

const int MAX_DEPTH = 40;
const int MAX_ITERATIONS = 250000;
const int IMAGE_SIZE = 2048;


std::vector<Matrix2D> matrices;


int main(int argc, char** argv)
{
    std::future<Matrix2D> instance1 = std::async(std::launch::async, generateBuddhabrotHistogram);
    std::future<Matrix2D> instance2 = std::async(std::launch::async, generateBuddhabrotHistogram);

    Matrix2D matrix1 = instance1.get();
    Matrix2D matrix2 = instance2.get();

    writeMatrixToPPM(matrix1, "image_pre1.ppm");
    writeMatrixToPPM(matrix2, "image_pre2.ppm");

    Matrix2D totalMatrix;
    initializeMatrix(totalMatrix);
    for (int x = 0; x < IMAGE_SIZE; x++)
        for (int y = 0; y < IMAGE_SIZE; y++)
            totalMatrix[x][y] = matrix1[x][y] + matrix2[x][y];

    writeMatrixToPPM(totalMatrix, "image.ppm");

    return EXIT_SUCCESS;
    //return EXIT_FAILURE;
}



Matrix2D generateBuddhabrotHistogram()
{
    Matrix2D matrix;
    initializeMatrix(matrix);

    fillMatrixWithBuddhabrot(matrix);
    return matrix;
}



void initializeMatrix(Matrix2D& matrix)
{
    for (std::size_t j = 0; j < IMAGE_SIZE; j++)
        matrix.push_back(std::vector<unsigned int>(IMAGE_SIZE, 0));
}



void fillMatrixWithBuddhabrot(Matrix2D& matrix)
{
    std::cout << "INSTANCE LAUNCHED" << std::endl;

    auto mersenneTwister = getMersenneTwister();
    std::uniform_real_distribution<float> randomFloat(-2, 2);

    std::vector<std::complex<float>> points;
    points.resize(MAX_DEPTH);

    for (int j = 0; j < MAX_ITERATIONS; j++)
    {
        float ptX = randomFloat(mersenneTwister);
        float ptY = randomFloat(mersenneTwister);

        std::complex<float> c(ptX, ptY);
        std::complex<float> z(0, 0);
        int iterations;
        for (iterations = 0; norm(z) < 4 && iterations < MAX_DEPTH; iterations++)
        {
            z = z * z + c;
            points.push_back(z);
        }

        if (iterations == MAX_DEPTH) //given point has not escaped
            for (const auto &point : points)
                updateCounter(matrix, point.real(), point.imag());
    }

    std::cout << "INSTANCE ENDED" << std::endl;
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



void writeMatrixToPPM(Matrix2D& matrix, std::string filename)
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
    ppm.open(filename, std::ofstream::out);

    ppm << "P2 " << IMAGE_SIZE << " " << IMAGE_SIZE << " 255" << std::endl;
    for (const auto &row : matrix)
    {
        for (const auto &cell : row)
        {
            float scale = cell / (float)sum / max;
            int grayscale = (int)(scale * 255);
            ppm << grayscale << " ";
        }
        ppm << std::endl;
    }

    ppm.close();
}



std::mt19937 getMersenneTwister()
{
    std::array<int, std::mt19937::state_size> seed_data;
    std::random_device r;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 mersenneTwister(seq);
    return mersenneTwister;
}
