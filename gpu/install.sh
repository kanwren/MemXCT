#!/bin/bash

sudo yum install -y tmux
sudo yum install -y epel-release
sudo yum install -y htop

sudo yum install -y centos-release-scl
sudo yum install -y devtoolset-9-gcc*

sudo yum group install "Development Tools" -y
sudo yum install kernel-devel-$(uname -r) kernel-headers-$(uname -r) -y
scl enable devtoolset-9 bash
module load mpi/hpcx
sudo rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
sudo yum install dkms -y
wget https://developer.download.nvidia.com/compute/cuda/11.1.1/local_installers/cuda-repo-rhel7-11-1-local-11.1.1_455.32.00-1.x86_64.rpm
sudo rpm -i cuda-repo-rhel7-11-1-local-11.1.1_455.32.00-1.x86_64.rpm
sudo yum clean all
sudo yum -y install nvidia-driver-latest-dkms cuda
sudo yum -y install cuda-drivers
sudo systemctl enable nvidia-persistenced
