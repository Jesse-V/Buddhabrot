#!/bin/sh
cmake .
cpus=$(grep -c ^processor /proc/cpuinfo)
if (make -j $cpus) then
    ./buddhabrot
    convert image.ppm -define png:bit-depth=16 -define png:color-type=0 -quality 100 image.png
fi
