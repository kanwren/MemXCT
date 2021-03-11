#!/bin/bash

# Install azcopy

cd ~
wget https://azcopyvnext.azureedge.net/release20201106/azcopy_linux_amd64_10.7.0.tar.gz
tar xvzf azcopy_linux_amd64_10.7.0.tar.gz
sudo mv azcopy_linux_amd64_10.7.0/azcopy /usr/local/bin

# Optional tools for convenience

sudo yum install -y tmux
sudo yum install -y epel-release
sudo yum install -y htop

# Install gcc-9.3.0

sudo yum install -y centos-release-scl
sudo yum install -y devtoolset-9-gcc*

# Update kernel

sudo yum group install "Development Tools" -y
sudo yum install kernel-devel-$(uname -r) kernel-headers-$(uname -r) -y

# Install cuda

sudo rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
sudo yum install dkms -y
wget https://developer.download.nvidia.com/compute/cuda/11.1.1/local_installers/cuda-repo-rhel7-11-1-local-11.1.1_455.32.00-1.x86_64.rpm
sudo rpm -i cuda-repo-rhel7-11-1-local-11.1.1_455.32.00-1.x86_64.rpm
sudo yum clean all
sudo yum -y install nvidia-driver-latest-dkms cuda
sudo yum -y install cuda-drivers
sudo systemctl enable nvidia-persistenced
