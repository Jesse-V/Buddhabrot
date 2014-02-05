#!/bin/sh
cmake .
cpus=$(grep -c ^processor /proc/cpuinfo)
if (make -j $cpus) then
    if (./buddhabrot) then
        convert image.ppm -quality 100 image.png
    fi
fi

#-define png:color-type=0
# -define png:bit-depth=16
