#!/bin/bash

#SBATCH --job-name="memxct"
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=120
#SBATCH --output="memxct-cds1-multinode.out"

set -euo pipefail

die() {
  >&2 echo "$1"
  exit 1
}

PREFIX="/shared/scratch/MemXCT"
EXE_PATH="$PREFIX/memxct"
[ ! -f "$EXE_PATH" ] && die "Executable not found: $EXE_PATH"

# module load mpi/hpcx || die "Failed to load HPC-X"
# module load mpi/mvapich2 || die "Failed to load MVAPICH2"
# echo "Using mpirun from $(which mpirun)"
# mpirun --version

dataset="CDS1"

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
  CDS1)
    export NUMTHE=750
    export NUMRHO=512
    ;;
  CDS2)
    export NUMTHE=375
    export NUMRHO=1024
    ;;
  CDS3)
    export NUMTHE=1501
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
  export SPATSIZE=32
  export SPECSIZE=32

  # block size
  export PROJBLOCK=512
  export BACKBLOCK=512

  # buffer size
  export PROJBUFF=8
  export BACKBUFF=8

export OMP_PLACES=sockets
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=120

hosts="$(scontrol show hostnames "$SLURM_JOB_NODELIST" | tr '\n' ',' | sed 's/,/:120,/g;s/,$//')"
echo "Using hosts: $hosts"

PKEY="$(cat /sys/class/infiniband/mlx5_0/ports/1/pkeys/* | grep -v 0000 | grep -v 0x7fff)"
PKEY="${PKEY/0x8/0x0}"
echo "PKEY: $PKEY"

mpirun -np 1 --host "$hosts" --map-by ppr:1:node:pe=120 --bind-to none -mca pml ucx --mca btl ^vader,tcp,openib -x UCX_NET_DEVICES=mlx5_0:1 -x UCX_IB_PKEY=$PKEY -x UCX_TLS=rc -x OMP_NUM_THREADS=120 "$EXE_PATH"

