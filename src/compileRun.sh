#!/bin/sh
cpus=$(grep -c ^processor /proc/cpuinfo)
if (mpic++ --std=c++11 main.cpp -O3 -o buddhabrot) then
    mpirun -np 2 ./buddhabrot
    convert image.ppm -define png:bit-depth=16 -define png:color-type=0 -quality 100 image.png
fi
