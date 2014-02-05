#!/bin/sh
use OpenMPI-GCC-4.8
if (mpic++ --std=c++11 main.cpp -O3 -o buddhabrot) then
    echo "Compilation success."
    exit 0
else
    echo "Compilation failure."
    exit 1
fi
