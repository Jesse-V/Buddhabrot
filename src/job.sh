#!/bin/bash
# The #PBS directives are the equivalent of passing the same argument on
# the qsub command line.

#PBS -N buddhabrot
#PBS -l nodes=2:ppn=4
#PBS -l walltime=00:05:30
#PBS -M jvictors@jessevictors.com

#PBS -j oe
#PBS -m abe

. /rc/tools/utils/dkinit
use OpenMPI-GCC-4.8

#LD_LIBRARY_PATH=/home/A01514050:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH

mpirun -np $PBS_NP buddhabrot/buddhabrot
convert buddhabrot/image.ppm -define png:bit-depth=16 -define png:color-type=0 -quality 100 buddhabrot/image.png
