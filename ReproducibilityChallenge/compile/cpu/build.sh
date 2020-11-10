#!/bin/bash

./install.sh
./setup.sh

export PATH=/opt/rh/devtoolset-9/root/usr/bin:$PATH
export LD_LIBRARY_PATH=/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:/opt/rh/devtoolset-9/root/usr/lib64/dyninst:/opt/rh/devtoolset-9/root/usr/lib/dyninst:/opt/rh/devtoolset-9/root/usr/lib64:/opt/rh/devtoolset-9/root/usr/lib:$LD_LIBRARY_PATH
module load mpi/hpcx

make clean all
cp memxct /shared/scratch/MemXCT
