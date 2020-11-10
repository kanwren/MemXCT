# doc

The artifact contains scripts for installing the necessary dependencies,
downloading and setting up the competition datasets, compiling the executables,
performing runs on the executables, and aggregating results into figures. Due to
the nature of MemXCT, most of the directories contain both CPU and GPU versions.

## Setup and Compiling

To install the necessary dependencies, download the competition datasets, and
compile MemXCT, go to `compile/cpu` or `compile/gpu` and run `./build.sh`. These
assume that you are on similar hardware, running CentOS 7. To only compile, see
the notes about necessary dependencies in `compile/README.md` and use the CPU or
GPU Makefile.

## Running

`run` provides scripts for single- and multi-node CPU and GPU runs, assuming you
are using the same dependencies (GCC, HPC-X Open MPI, and CUDA 11 on GPUs). It
exports scripts `run/cpu-runs.sh` and `run/gpu-runs.sh` that use Slurm to queue
many single- and multi-node CPU/GPU runs, generating 10 output runs for
CDS1/CDS2 and multiple numbers of nodes. We perform 10 runs for each data point
since the performance has a particularly high variance, especially on CPUs.
There are also instructions for performing individual runs; these can be done
with `cpu/singlenode.sh`, `cpu/multinode.sh`, `gpu/singlenode.sh`, and
`gpu/multinode.sh`. All of these take as arguments the input dataset (`CDS1-3`),
the tile size, the block size, and the buffer size. For runs with
`multinode.sh`, the appropriate scheduler command must be invoked on the script.
For example, to do a multi-node CPU run using Slurm:

```
$ sbatch --job-name "memxct" --output "memxct.out" --nodes 4 --ntasks-per-node 32 \
    cpu/multinode.sh CDS3 128 128 8
```

## Figures

TODO

