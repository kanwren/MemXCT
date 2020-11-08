#!/bin/bash

#PBS -N memxct
#PBS -l nodes=1:ppn=120
#PBS -j oe
#PBS -o memxct-ads4.out

set -euo pipefail

die() {
  >&2 echo "$1"
  exit 1
}

PREFIX="/shared/scratch/MemXCT"
EXE_PATH="$PREFIX/memxct"
[ ! -f "$EXE_PATH" ] && die "Executable not found: $EXE_PATH"

module load mpi/hpcx || die "Failed to load HPC-X"
echo "Using mpirun from $(which mpirun)"
mpirun --version

dataset="ADS4"

# set box dimensions according to input dataset
case "$dataset" in
    ADS1)
        export NUMTHE=360
        export NUMRHO=256
        ;;
    ADS2)
        export NUMTHE=750
        export NUMRHO=512
        ;;
    ADS3)
        export NUMTHE=1500
        export NUMRHO=1024
        ;;
    ADS4)
        export NUMTHE=2400
        export NUMRHO=2048
        ;;
    *)
        die "Error: unrecognized dataset '$dataset'"
        ;;
esac

# i/o files

export THEFILE="$PREFIX/data/${dataset}_theta.bin"
export SINFILE="$PREFIX/data/${dataset}_sinogram.bin"
export OUTFILE="$PREFIX/recon_${dataset}.bin"

[ ! -f "$THEFILE" ] && die "Theta file not found: $THEFILE"
[ ! -f "$SINFILE" ] && die "Sinogram file not found: $SINFILE"

# Constants
    # solver data
    export NUMITER=24       # constant
    # domain information
    export PIXSIZE=1        # constant

# Tuning parameters:
    # tile size (must be power of two)
    export SPATSIZE=128
    export SPECSIZE=128

    # block size
    export PROJBLOCK=128
    export BACKBLOCK=128

    # buffer size
    export PROJBUFF=8
    export BACKBUFF=8

export OMP_PLACES=sockets
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=4

export UCX_TLS=rc

hosts="$(sort -u $PBS_NODEFILE | awk -F. '{print $1}' | tr '\n' ',' | sed 's/,$//')"

echo "Using hosts: $hosts"

mpirun -np 1 --bind-to none "$EXE_PATH"
