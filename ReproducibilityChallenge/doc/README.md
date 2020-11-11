# doc

The artifact contains scripts for installing the necessary dependencies,
downloading and setting up the competition datasets, compiling the executables,
performing runs on the executables, and aggregating results into figures. Due to
the nature of MemXCT, most of the directories contain both CPU and GPU versions.

## Setup and Compiling

To install the necessary dependencies, download the competition datasets, and
compile MemXCT, go to `compile/cpu` or `compile/gpu` and run `./build.sh`. These
assume that you are on similar hardware, running CentOS 7. If not on CentOS, see
the notes about necessary dependencies in `compile/README.md` and use the CPU or
GPU Makefile.

`compile/` additionally contains:

- a GCC vectorization report (`vectorization-report.log`)
  - Note that vectorization on AMD is very limited, so most of the vectorization
    does not fire
- system information files:
  - CPU cluster master node system information (`cpu-master-node-sysinfo.txt`)
  - CPU cluster execute node system information (`cpu-execute-node-sysinfo.txt`)
  - GPU cluster master node system information (`gpu-master-node-sysinfo.txt`)
  - GPU cluster execute node system information (`gpu-execute-node-sysinfo.txt`)

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

To view the figures, open the images within the output directory. To reproduce these figures yourself, go into the scripts folder and open the ipynb (jupyter notebook) that corresponds to which figures you want to generate. If you are trying to reproduce the strong scaling experiments (any of the following: CPU-cds1, CPU-cds2, GPU-cds1, or GPU-cds2) use the file `StrongScaling_plots.ipynb`. If attempting to create the performance plots (for single node CPU or GPU) use `singlenode_performance(gpu_cpu)_plots.ipynb`. I recommend opening these in google colab, as that will ensure all dependencies are setup and ready. The data inside these files is populated by hand, but we calculated the numbers by manually looking at our log files (getting them into csv using vim) and parsing what we needed from the csvs. 

