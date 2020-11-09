#!/bin/bash

# Download datasets

PREFIX="/shared/scratch/MemXCT"

mkdir -p "$PREFIX/data"

cd "$PREFIX/data"
azcopy copy 'https://scc20input.blob.core.windows.net/memxct' '.' --recursive
tar xvf memxct/MEMXCT.tar

