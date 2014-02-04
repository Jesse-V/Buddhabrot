#!/bin/sh
if (./compile.sh) then
    mpirun -np 2 ./buddhabrot
    convert buddhabrot/image.ppm -define png:bit-depth=16 -define png:color-type=0 -quality 100 buddhabrot/image.png
fi
