# compile

There are two different directories, `cpu/` and `gpu/`, for compiling the CPU
and GPU versions of the code, respectively. CPU and GPU code are taken from
[https://github.com/merthidayetoglu/MemXCT-CPU](https://github.com/merthidayetoglu/MemXCT-CPU)
and
[https://github.com/merthidayetoglu/MemXCT-GPU](https://github.com/merthidayetoglu/MemXCT-GPU)
respectively.

This directory also contains:

- a GCC vectorization report (`vectorization-report.log`)
  - Note that vectorization on AMD is very limited, so most of the vectorization
    does not fire
- system information files:
  - CPU cluster master node system information (`cpu-master-node-sysinfo.txt`)
  - CPU cluster execute node system information (`cpu-execute-node-sysinfo.txt`)
  - GPU cluster master node system information (`gpu-master-node-sysinfo.txt`)
  - GPU cluster execute node system information (`gpu-execute-node-sysinfo.txt`)

# CPU

The CPU version is compiled with `GCC 9.3.1 20200408 (Red Hat 9.3.1-2)`, which
on our VMs is supplied by `devtoolset-9-gcc` from the CentOS Software
Collections. We use Open MPI `4.0.4rc3`, which is supplied by HPC-X
(specifically, `hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64`).
HPC-X in turn is included by default in our CentOS VM image,
`OpenLogic:CentOS-HPC:7.7:latest`.

In order to compile the CPU version of the code, simply run `build.sh` if you're
on CentOS. If not on CentOS, the script does the following for reference:

- Install the necessary dependencies
  - The above dependencies may be installed on CentOS by running the `install.sh` script
- Load the necessary dependencies
  - Runs the following:
    - `scl enable devtoolset-9 bash`
    - `module load mpi/hpcx`
- Run the included Makefile with `make` to produce the `memxct` executable
- The run scripts later expect the input data files and executable to
  be in `/shared/scratch/MemXCT`. In order to set this up, it will run
  `setup.sh` to set up the directory and copy the data files, and then `cp memxct /shared/scratch/MemXCT`.

# GPU

The GPU version is compiled with `GCC 9.3.1 20200408 (Red Hat 9.3.1-2)`, which
on our VMs is supplied by `devtoolset-9-gcc` from the CentOS Software
Collections. We use Open MPI `4.0.4rc3`, which is supplied by HPC-X
(specifically, `hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64`).
HPC-X in turn is included by default in our CentOS VM image,
`OpenLogic:CentOS-HPC:7.7:7.7.2020062600`.

For the GPU version, we additionally use CUDA 11.1.1, which we install from the
Nvidia website as RPM packages for CentOS. Note that CUDA installation
significantly complicates the installation process, requiring a kernel udpate
and enabling a daemon.

In order to compile the GPU version of the code, simply run `build.sh` if you're
on CentOS. If not on CentOS, the script does the following for reference:

- Install the necessary dependencies
  - The above dependencies may be installed on CentOS by running the `install.sh` script
- Load the necessary dependencies
  - Runs the following:
    - `scl enable devtoolset-9 bash`
    - `module load mpi/hpcx`
    - `source cuda-env.sh`
- Run the included Makefile with `make` to produce the `memxct` executable
- The run scripts later expect the input data files and executable to
  be in `/shared/scratch/MemXCT`. In order to set this up, it will run
  `setup.sh` to set up the directory and copy the data files, and then `cp memxct /shared/scratch/MemXCT`.

