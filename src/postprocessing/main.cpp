
#include "main.hpp"
#include <complex>
#include <fstream>
#include <future>
#include <chrono>
#include <iostream>


const int IMAGE_SIZE = 4096; //size of the resulting image, N * N


float getIntensity(float value, float lowerRange, float upperRange, bool maxFull)
{
    float intensity;
    if (value < lowerRange)
        intensity = 0;
    else if (value > upperRange)
        intensity = maxFull ? 1 : 0;
    else
        intensity = (value - lowerRange) / (upperRange - lowerRange);

    return intensity;

}

int main(int argc, char** argv)
{
    std::cout << "Reading... ";
    std::cout.flush();

    std::ifstream fin;
    fin.open(std::string("regular_histogram.txt"), std::ofstream::out);
    Matrix2D matrix;
    for (int j = 0; j < IMAGE_SIZE; j++)
    {
        std::vector<unsigned long> row;
        for (int k = 0; k < IMAGE_SIZE; k++)
        {
            unsigned long value;
            fin >> value;
            row.push_back(value);
        }
        matrix.push_back(row);
    }
    fin.close();

    std::cout << "done." << std::endl;
    std::cout.flush();

    unsigned long temp = 0;
    for (const auto &row : matrix)
        for (const auto &cell : row)
            temp += cell;
    float sum = (float)temp;

    float maxDensity = 0;
    for (const auto &row : matrix)
        for (const auto &cell : row)
            if (cell / sum > maxDensity)
                maxDensity = cell / sum;

    const float LOWER_GREEN = 0.00007f * maxDensity, UPPER_GREEN = maxDensity * 0.0002f;
    const float LOWER_BLUE = maxDensity * 0.0001f, UPPER_BLUE = maxDensity * 0.00045f;
    const float LOWER_RED = maxDensity * 0.000325f, UPPER_RED = maxDensity * 0.00075f;

    //const float LOWER_GREEN = 0, UPPER_GREEN = maxDensity * 0.008f;
    //const float LOWER_BLUE = maxDensity * 0.000f, UPPER_BLUE = maxDensity * 0.17f;
    //const float LOWER_RED = maxDensity * 0.115f, UPPER_RED = maxDensity * 0.25f;

    //const float LOWER_GREEN = 0, UPPER_GREEN = maxDensity * 0.008f;
    //const float LOWER_BLUE = UPPER_GREEN, UPPER_BLUE = maxDensity * 0.115f;
    //const float LOWER_RED = UPPER_BLUE, UPPER_RED = maxDensity * 0.2f;

    std::cout << "Writing... ";
    std::cout.flush();

    std::ofstream fout;
    fout.open(std::string("image.ppm"), std::ofstream::out);

    fout << "P3 " << IMAGE_SIZE << " " << IMAGE_SIZE << " 255" << std::endl;
    for (const auto &row : matrix)
    {
        for (const auto &cell : row)
        {
            float gradiant = cell / sum;

            float red = getIntensity(gradiant, LOWER_RED, UPPER_RED, true);
            float green = getIntensity(gradiant, LOWER_GREEN, UPPER_GREEN, false);
            float blue = getIntensity(gradiant, LOWER_BLUE, UPPER_BLUE, false);

            fout << (int)(red * 255) << " " << (int)(green * 255) << " " << (int)(blue * 255) << " ";
        }
        fout << std::endl;
    }
    fout.close();

    std::cout << "done." << std::endl;
    std::cout.flush();



    return EXIT_SUCCESS;
    //return EXIT_FAILURE;
}
