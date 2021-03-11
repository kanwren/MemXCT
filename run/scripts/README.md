# run/scripts

The CPU and GPU versions of the application use slightly different scripts to
run. Both rely on the note mentioned in the build script README: the application
should be at `/shared/scratch/MemXCT/memxct`, and the datasets should be in
`/shared/scratch/MemXCT/data`.

The reconstructed tomograms will be placed in `/shared/scratch/MemXCT`. The
output logs will be placed where you request; with the run scripts provided by
default, they will go into the current directory, and must be copied manually.

# CPU

Assuming you are using Slurm on a cluster with a master node and multiple
execute nodes, `cpu-runs.sh` will perform a set of 10 single node runs, as well
as 10 multi-node runs for each dataset on 1, 2, and 4 nodes. It uses tuning
parameters slightly different from those in the paper, which were found by a
small traversal of the tuning parameter search space. It performs 10 of each run
as the performance variance is rather high, so we compute an average for the
data shown in the reproducibility report.

To do a single run:

- If you are on a cluster with a single node and no scheduler, simply run `cpu/singlenode.sh <dataset> <tilesize> <blocksize> <buffersize>`.
  - For example, `cpu/singlenode.sh CDS2 128 128 8`
- If you are on a cluster with multiple nodes and a scheduler, use the
    appropriate scheduler command to run `cpu/multinode.sh` with the same
    arguments as above
  - For example, with Slurm, `sbatch --job-name="memxct" --output="memxct.out" --nodes=1 --ntasks-per-node=32 cpu/multinode.sh CDS3 128 128 8`

# GPU

Assuming you are using Slurm on a cluster with a master node and multiple
execute nodes, `gpu-runs.sh` will perform a set of 10 single node runs, as well
as 10 multi-node runs for each dataset on 1, 2, and 4 nodes. It uses tuning
parameters slightly different from those in the paper, which were found by a
small traversal of the tuning parameter search space. It performs 10 of each run
as the performance variance is rather high (though less so than on CPUs), so we
compute an average for the data shown in the reproducibility report.

To do a single run:

- If you are on a cluster with a single node and no scheduler, simply run `gpu/singlenode.sh <dataset> <tilesize> <blocksize> <buffersize>`.
  - For example, `gpu/singlenode.sh CDS2 128 128 8`
- If you are on a cluster with multiple nodes and a scheduler, use the
    appropriate scheduler command to run `gpu/multinode.sh` with the same
    arguments as above
  - For example, with Slurm, `sbatch --job-name="memxct" --output="memxct.out" --nodes=1 --ntasks-per-node=32 gpu/multinode.sh CDS3 128 128 8`

# Environment

The specific environment should not be necessary to perform runs; the scripts
should load the necessary dependencies. For completeness, here is the complete
output of `env` on CPU runs:

```
MANPATH=/opt/rh/devtoolset-9/root/usr/share/man:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/share/man:/usr/share/man:/usr/local/share/man
XDG_SESSION_ID=5
SHMEM_HOME=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
HOSTNAME=ip-0A000011
SELINUX_ROLE_REQUESTED=
HPCX_MPI_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
SHELL=/bin/bash
TERM=screen-256color
HISTSIZE=1000
SSH_CLIENT=71.59.9.252 45584 22
LIBRARY_PATH=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/nccl_rdma_sharp_plugin/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/sharp/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hcoll/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hmc/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx/lib
SELINUX_USE_CURRENT_RANGE=
OLDPWD=/mnt/exports/shared/home/ccuser
SSH_TTY=/dev/pts/0
CYCLECLOUD_BOOTSTRAP=/opt/cycle/jetpack/system/bootstrap
X_SCLS=devtoolset-9 
HPCX_NCCL_RDMA_SHARP_PLUGIN_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/nccl_rdma_sharp_plugin
USER=ccuser
HPCX_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64
OPAL_PREFIX=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
PMIX_INSTALL_PREFIX=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
PCP_DIR=/opt/rh/devtoolset-9/root
LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
LD_LIBRARY_PATH=/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:/opt/rh/devtoolset-9/root/usr/lib64/dyninst:/opt/rh/devtoolset-9/root/usr/lib/dyninst:/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/nccl_rdma_sharp_plugin/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/sharp/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hcoll/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hmc/lib:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx/lib/ucx:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx/lib
CPATH=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/include:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hmc/include:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx/include:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/sharp/include:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hcoll/include
HPCX_HOME=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64
HPCX_CLUSTERKIT_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/clusterkit
HPCX_OSU_CUDA_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/tests/osu-micro-benchmarks-5.6.2-cuda
HPCX_IPM_LIB=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/tests/ipm-2.0.6/lib/libipm.so
HPCX_SHARP_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/sharp
PATH=/opt/rh/devtoolset-9/root/usr/bin:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/bin:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/clusterkit/bin:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/tests/imb:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hcoll/bin:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/opt/cycle/jetpack/bin:/mnt/exports/shared/home/ccuser/.local/bin:/mnt/exports/shared/home/ccuser/bin:/opt/cycle/jetpack/bin
MAIL=/var/spool/mail/ccuser
HPCX_HCOLL_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hcoll
PWD=/mnt/exports/shared/home/ccuser/Author-Kit
OSHMEM_HOME=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
_LMFILES_=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/modulefiles/hpcx:/usr/share/Modules/modulefiles/mpi/hpcx
LANG=en_US.UTF-8
HPCX_HMC_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hmc
HPCX_IPM_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/tests/ipm-2.0.6
HPCX_UCX_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx
MODULEPATH=/usr/share/Modules/modulefiles:/etc/modulefiles
LOADEDMODULES=hpcx:mpi/hpcx
SELINUX_LEVEL_REQUESTED=
OMPI_HOME=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
HISTCONTROL=ignoredups
HPCX_OSU_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/tests/osu-micro-benchmarks-5.6.2
HOME=/mnt/exports/shared/home/ccuser
SHLVL=4
HPCX_OSHMEM_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
LOGNAME=ccuser
SSH_CONNECTION=71.59.9.252 45584 10.0.0.17 22
MODULESHOME=/usr/share/Modules
MPI_HOME=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi
LESSOPEN=||/usr/bin/lesspipe.sh %s
PKG_CONFIG_PATH=/opt/rh/devtoolset-9/root/usr/lib64/pkgconfig:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/lib/pkgconfig:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ucx/lib/pkgconfig:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/sharp/lib/pkgconfig:/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/hcoll/lib/pkgconfig
CYCLECLOUD_HOME=/opt/cycle/jetpack
INFOPATH=/opt/rh/devtoolset-9/root/usr/share/info
XDG_RUNTIME_DIR=/run/user/20002
HPCX_MPI_TESTS_DIR=/opt/hpcx-v2.7.0-gcc-MLNX_OFED_LINUX-5.1-0.6.6.0-redhat7.7-x86_64/ompi/tests
BASH_FUNC_module()=() {  eval `/usr/bin/modulecmd bash $*`
}
_=/usr/bin/env
```

And here is the complete output of `env` on GPU runs:

```
MANPATH=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/share/man:/opt/rh/devtoolset-9/root/usr/share/man:
XDG_SESSION_ID=5
SHMEM_HOME=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
HOSTNAME=ip-0A00000A
SELINUX_ROLE_REQUESTED=
HPCX_MPI_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
TERM=screen-256color
SHELL=/bin/bash
HISTSIZE=1000
SSH_CLIENT=71.59.9.252 60056 22
LIBRARY_PATH=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/lib:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/sharp/lib:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/hcoll/lib:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx/lib
SELINUX_USE_CURRENT_RANGE=
OLDPWD=/mnt/exports/shared/home/ccuser
X_SCLS=devtoolset-9 
CYCLECLOUD_BOOTSTRAP=/opt/cycle/jetpack/system/bootstrap
SSH_TTY=/dev/pts/0
PCP_DIR=/opt/rh/devtoolset-9/root
USER=ccuser
HPCX_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64
OPAL_PREFIX=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
PMIX_INSTALL_PREFIX=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
LD_LIBRARY_PATH=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/lib:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/sharp/lib:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/hcoll/lib:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx/lib/ucx:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx/lib:/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:/opt/rh/devtoolset-9/root/usr/lib64/dyninst:/opt/rh/devtoolset-9/root/usr/lib/dyninst:/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib
LS_COLORS=rs=0:di=38;5;27:ln=38;5;51:mh=44;38;5;15:pi=40;38;5;11:so=38;5;13:do=38;5;5:bd=48;5;232;38;5;11:cd=48;5;232;38;5;3:or=48;5;232;38;5;9:mi=05;48;5;232;38;5;15:su=48;5;196;38;5;15:sg=48;5;11;38;5;16:ca=48;5;196;38;5;226:tw=48;5;10;38;5;16:ow=48;5;10;38;5;21:st=48;5;21;38;5;15:ex=38;5;34:*.tar=38;5;9:*.tgz=38;5;9:*.arc=38;5;9:*.arj=38;5;9:*.taz=38;5;9:*.lha=38;5;9:*.lz4=38;5;9:*.lzh=38;5;9:*.lzma=38;5;9:*.tlz=38;5;9:*.txz=38;5;9:*.tzo=38;5;9:*.t7z=38;5;9:*.zip=38;5;9:*.z=38;5;9:*.Z=38;5;9:*.dz=38;5;9:*.gz=38;5;9:*.lrz=38;5;9:*.lz=38;5;9:*.lzo=38;5;9:*.xz=38;5;9:*.bz2=38;5;9:*.bz=38;5;9:*.tbz=38;5;9:*.tbz2=38;5;9:*.tz=38;5;9:*.deb=38;5;9:*.rpm=38;5;9:*.jar=38;5;9:*.war=38;5;9:*.ear=38;5;9:*.sar=38;5;9:*.rar=38;5;9:*.alz=38;5;9:*.ace=38;5;9:*.zoo=38;5;9:*.cpio=38;5;9:*.7z=38;5;9:*.rz=38;5;9:*.cab=38;5;9:*.jpg=38;5;13:*.jpeg=38;5;13:*.gif=38;5;13:*.bmp=38;5;13:*.pbm=38;5;13:*.pgm=38;5;13:*.ppm=38;5;13:*.tga=38;5;13:*.xbm=38;5;13:*.xpm=38;5;13:*.tif=38;5;13:*.tiff=38;5;13:*.png=38;5;13:*.svg=38;5;13:*.svgz=38;5;13:*.mng=38;5;13:*.pcx=38;5;13:*.mov=38;5;13:*.mpg=38;5;13:*.mpeg=38;5;13:*.m2v=38;5;13:*.mkv=38;5;13:*.webm=38;5;13:*.ogm=38;5;13:*.mp4=38;5;13:*.m4v=38;5;13:*.mp4v=38;5;13:*.vob=38;5;13:*.qt=38;5;13:*.nuv=38;5;13:*.wmv=38;5;13:*.asf=38;5;13:*.rm=38;5;13:*.rmvb=38;5;13:*.flc=38;5;13:*.avi=38;5;13:*.fli=38;5;13:*.flv=38;5;13:*.gl=38;5;13:*.dl=38;5;13:*.xcf=38;5;13:*.xwd=38;5;13:*.yuv=38;5;13:*.cgm=38;5;13:*.emf=38;5;13:*.axv=38;5;13:*.anx=38;5;13:*.ogv=38;5;13:*.ogx=38;5;13:*.aac=38;5;45:*.au=38;5;45:*.flac=38;5;45:*.mid=38;5;45:*.midi=38;5;45:*.mka=38;5;45:*.mp3=38;5;45:*.mpc=38;5;45:*.ogg=38;5;45:*.ra=38;5;45:*.wav=38;5;45:*.axa=38;5;45:*.oga=38;5;45:*.spx=38;5;45:*.xspf=38;5;45:
CPATH=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/include:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx/include:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/sharp/include:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/hcoll/include
HPCX_HOME=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64
HPCX_CLUSTERKIT_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/clusterkit
HPCX_IPM_LIB=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/tests/ipm-2.0.6/lib/libipm.so
HPCX_SHARP_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/sharp
MAIL=/var/spool/mail/ccuser
PATH=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/bin:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/clusterkit/bin:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/tests/imb:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/hcoll/bin:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx/bin:/opt/rh/devtoolset-9/root/usr/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/opt/cycle/jetpack/bin:/opt/ibutils/bin:/mnt/exports/shared/home/ccuser/.local/bin:/mnt/exports/shared/home/ccuser/bin:/opt/cycle/jetpack/bin:/opt/cycle/jetpack/bin
HPCX_HCOLL_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/hcoll
PWD=/mnt/exports/shared/home/ccuser/Author-Kit
OSHMEM_HOME=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
_LMFILES_=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/modulefiles/hpcx:/usr/share/Modules/modulefiles/mpi/hpcx
LANG=en_US.UTF-8
HPCX_IPM_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/tests/ipm-2.0.6
HPCX_UCX_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx
MODULEPATH=/usr/share/Modules/modulefiles:/etc/modulefiles
LOADEDMODULES=hpcx:mpi/hpcx
SELINUX_LEVEL_REQUESTED=
OMPI_HOME=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
HISTCONTROL=ignoredups
HPCX_OSU_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/tests/osu-micro-benchmarks-5.3.2
SHLVL=7
HOME=/mnt/exports/shared/home/ccuser
HPCX_OSHMEM_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
LOGNAME=ccuser
SSH_CONNECTION=71.59.9.252 60056 10.0.0.10 22
MODULESHOME=/usr/share/Modules
MPI_HOME=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi
PKG_CONFIG_PATH=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/lib/pkgconfig:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ucx/lib/pkgconfig:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/sharp/lib/pkgconfig:/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/hcoll/lib/pkgconfig:/opt/rh/devtoolset-9/root/usr/lib64/pkgconfig
LESSOPEN=||/usr/bin/lesspipe.sh %s
INFOPATH=/opt/rh/devtoolset-9/root/usr/share/info
CYCLECLOUD_HOME=/opt/cycle/jetpack
XDG_RUNTIME_DIR=/run/user/20002
HPCX_MPI_TESTS_DIR=/opt/hpcx-v2.6.0-gcc-MLNX_OFED_LINUX-5.0-1.0.0.0-redhat7.7-x86_64/ompi/tests
BASH_FUNC_module()=() {  eval `/usr/bin/modulecmd bash $*`
}
_=/usr/bin/env
```

