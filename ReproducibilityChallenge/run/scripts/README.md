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
MANPATH=/opt/rh/devtoolset-9/root/usr/share/man:
XDG_SESSION_ID=6
HOSTNAME=ip-0A000008
SELINUX_ROLE_REQUESTED=
TERM=screen
SHELL=/bin/bash
HISTSIZE=1000
SSH_CLIENT=71.59.9.252 48938 22
SELINUX_USE_CURRENT_RANGE=
OLDPWD=/mnt/exports/shared/home/ccuser/MemXCT/ReproducibilityChallenge/run
X_SCLS=devtoolset-9
SSH_TTY=/dev/pts/0
CYCLECLOUD_BOOTSTRAP=/opt/cycle/jetpack/system/bootstrap
PCP_DIR=/opt/rh/devtoolset-9/root
USER=ccuser
LD_LIBRARY_PATH=/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:/opt/rh/devtoolset-9/root/usr/lib64/dyninst:/opt/rh/devtoolset-9/root/usr/lib/dyninst:/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=01;05;37;41:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=01;36:*.au=01;36:*.flac=01;36:*.mid=01;36:*.midi=01;36:*.mka=01;36:*.mp3=01;36:*.mpc=01;36:*.ogg=01;36:*.ra=01;36:*.wav=01;36:*.axa=01;36:*.oga=01;36:*.spx=01;36:*.xspf=01;36:
TMUX=/tmp/tmux-20002/default,5780,0
PATH=/opt/rh/devtoolset-9/root/usr/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/opt/cycle/jetpack/bin:/mnt/exports/shared/home/ccuser/.local/bin:/mnt/exports/shared/home/ccuser/bin:/opt/cycle/jetpack/bin:/mnt/exports/shared/home/ccuser/.local/bin:/mnt/exports/shared/home/ccuser/bin:/opt/cycle/jetpack/bin:/opt/cycle/jetpack/bin
MAIL=/var/spool/mail/ccuser
PWD=/mnt/exports/shared/home/ccuser/MemXCT/ReproducibilityChallenge/run/scripts
LANG=en_US.UTF-8
MODULEPATH=/usr/share/Modules/modulefiles:/etc/modulefiles
TMUX_PANE=%2
SELINUX_LEVEL_REQUESTED=
HISTCONTROL=ignoredups
SHLVL=8
HOME=/mnt/exports/shared/home/ccuser
LOGNAME=ccuser
SSH_CONNECTION=71.59.9.252 48938 10.0.0.8 22
MODULESHOME=/usr/share/Modules
PKG_CONFIG_PATH=/opt/rh/devtoolset-9/root/usr/lib64/pkgconfig
LESSOPEN=||/usr/bin/lesspipe.sh %s
INFOPATH=/opt/rh/devtoolset-9/root/usr/share/info
CYCLECLOUD_HOME=/opt/cycle/jetpack
XDG_RUNTIME_DIR=/run/user/20002
BASH_FUNC_module()=() { eval `/usr/bin/modulecmd bash $*` }
_=/usr/bin/env
```

And here is the complete output of `env` on GPU runs:

```
TODO
```

