
#include "main.hpp"
#include <fstream>
#include <vector>
#include <iostream>

const int MAX_ITERATIONS = 100;
const std::size_t IMAGE_SIZE = 10;

typedef std::vector<std::vector<unsigned short>> Matrix2D;


int main(int argc, char** argv)
{
    Matrix2D matrix;
    for (std::size_t j = 0; j < IMAGE_SIZE; j++)
        matrix.push_back(std::vector<unsigned short>(IMAGE_SIZE, 0));

    std::cout << "Printing..." << std::endl;
    for (const auto &row : matrix)
    {
        for (const auto &cell : row)
            std::cout << cell << " ";
        std::cout << std::endl;
    }

    writePPM();


    return EXIT_SUCCESS;
    //return EXIT_FAILURE;
}



void getColorAt(float ptX, float ptY, int& r, int& g, int& b)
{
    /*double ptYSq = pt.y * pt.y;
    double xOff = pt.x - 0.25;
    double q = Math.pow(xOff, 2) + ptYSq;
    if (q * (q + xOff) < ptYSq / 4)
            return Color.BLACK; //http://en.wikipedia.org/wiki/Mandelbrot_fractal#Optimizations
*/
    float x = 0, xSq = 0, y = 0, ySq = 0;
    int iterations;
    for (iterations = 0; iterations < MAX_ITERATIONS && (xSq + ySq <= 64); iterations++)
    {
            y = 2 * x * y + ptY;
            x = xSq - ySq + ptX;
            xSq = x * x;
            ySq = y * y;
    }

    if (iterations == MAX_ITERATIONS)
    {
        r = g = b = 0;
    }
    else
    {
        r = g = b = 1;
        /*
        double mu = iterations - Math.log(Math.log(xSq + ySq)) / LOG_2;
        float sin = (float)Math.sin(mu / viewport.getColoring()) / 2 + 0.5f;
        float cos = (float)Math.cos(mu / viewport.getColoring()) / 2 + 0.5f;
        return new Color(cos, cos, sin);*/
    }
}



void writePPM()
{
    std::ofstream ppm;
    ppm.open("image.ppm", std::ofstream::out);

    ppm << "P3 2 2 255" << std::endl;
    ppm << "255 0 0 0 255 0" << std::endl;
    ppm << "0 0 255 0 255 255" << std::endl;

    ppm.close();
}
