#!/bin/bash
# The #PBS directives are the equivalent of passing the same argument on
# the qsub command line.

#PBS -N buddhabrot
#PBS -l nodes=5:ppn=5
#PBS -l walltime=06:05:00
#PBS -M jvictors@jessevictors.com

#PBS -j oe
#PBS -m abe

. /rc/tools/utils/dkinit
use OpenMPI-GCC-4.8

#LD_LIBRARY_PATH=/home/A01514050:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH

mpirun -np $PBS_NP buddhabrot_raw/buddhabrot
