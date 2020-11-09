#!/bin/bash

# Install azcopy

cd ~
wget https://azcopyvnext.azureedge.net/release20201106/azcopy_linux_amd64_10.7.0.tar.gz
tar xvzf azcopy_linux_amd64_10.7.0.tar.gz
mv azcopy_linux_amd64_10.7.0/azcopy /usr/local/bin

# Optional tools for convenience

sudo yum install -y tmux
sudo yum install -y epel-release
sudo yum install -y htop

# Install gcc-9.3.0

sudo yum install -y centos-release-scl
sudo yum install -y devtoolset-9-gcc*
