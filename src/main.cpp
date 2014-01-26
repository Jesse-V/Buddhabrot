
#include "main.hpp"
#include <complex>
#include <fstream>
#include <future>
#include <chrono>
#include <iostream>


const int MAX_THREADS = 3;
const int MAX_DEPTH = 250; //max iterations to follow the point
const int SECONDS_TO_WORK = 600; //how long each thread should run for
const int ITERATION_WORK_BURST = 5000; //how many iterations to go before checking the clock
const int IMAGE_SIZE = 1024; //size of the resulting image, N * N


int main(int argc, char** argv)
{
    std::vector<std::future<Matrix2D>> buddhabrots;
    for (int threadID = 0; threadID < MAX_THREADS; threadID++)
        buddhabrots.push_back(std::async(std::launch::async, generateBuddhabrotHistogram));

    std::vector<Matrix2D> matrices;
    for (int threadID = 0; threadID < MAX_THREADS; threadID++)
        matrices.push_back(buddhabrots[threadID].get());

    std::cout << "Writing pre-images... ";
    for (int threadID = 0; threadID < MAX_THREADS; threadID++)
    {
        std::stringstream filename;
        filename << "image_pre" << threadID << ".ppm";
        writeMatrixToPPM(matrices[threadID], filename.str());
    }
    std::cout << "done" << std::endl;

    std::cout << "Writing final image... ";
    Matrix2D totalMatrix;
    initializeMatrix(totalMatrix);
    for (int x = 0; x < IMAGE_SIZE; x++)
    {
        for (int y = 0; y < IMAGE_SIZE; y++)
        {
            for (int threadID = 0; threadID < MAX_THREADS; threadID++)
            {
                auto old = totalMatrix[x][y];
                totalMatrix[x][y] += matrices[threadID][x][y];
                if (totalMatrix[x][y] < old)
                    std::cout << "Possible numerical rollover during sum!" << std::endl;
            }
        }
    }

    writeMatrixToPPM(totalMatrix, "image.ppm");
    std::cout << "done. Program complete." << std::endl;

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
    std::stringstream initialMessage;
    initialMessage << "Instance Launched. Expect shutdown in " << SECONDS_TO_WORK << " seconds." << std::endl;
    std::cout << initialMessage.str();

    using namespace std::chrono;

    auto mersenneTwister = getMersenneTwister();
    std::uniform_real_distribution<float> randomFloat(-2, 2);

    std::vector<std::complex<float>> points;
    points.resize(MAX_DEPTH);

    int totalIterations = 0;
    auto start = steady_clock::now();
    while (duration_cast<seconds>(steady_clock::now() - start).count() < SECONDS_TO_WORK)
    {
        for (int j = 0; j < ITERATION_WORK_BURST; j++)
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

            totalIterations += ITERATION_WORK_BURST;
        }
    }

    std::cout << "Time expired, " << totalIterations << " iterations complete." << std::endl;
}



void updateCounter(Matrix2D& matrix, float fractalX, float fractalY)
{
    int x = (int)((fractalX + 2) / 4 * IMAGE_SIZE);
    int y = (int)((fractalY + 2) / 4 * IMAGE_SIZE);

    if (x >= 0 && x < IMAGE_SIZE && y >= 0 && y < IMAGE_SIZE)
    {
        auto old = matrix[(std::size_t)x][(std::size_t)y]++;
        if (matrix[(std::size_t)x][(std::size_t)y] < old)
            std::cout << "Possible numerical rollover during update!" << std::endl;
    }
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
            ppm << (int)(scale * 255) << " ";
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
