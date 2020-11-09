#!/bin/bash

for d in (CDS1 CDS2 CDS3); do
	sbatch --jobname "memxct-singlenode-$d" --nodes=1 --ntasks-per-node=60 --output="memxct-singlenode-$d.out" cpu/singlenode.sh "$d" 32 512 8
	sbatch --jobname "memxct-singlenode-$d" --nodes=1 --ntasks-per-node=60 --output="memxct-singlenode-$d.out" cpu/singlenode.sh "$d" 32 512 8
	sbatch --jobname "memxct-singlenode-$d" --nodes=1 --ntasks-per-node=60 --output="memxct-singlenode-$d.out" cpu/singlenode.sh "$d" 32 512 8
done

for d in (CDS1 CDS2); do
	sbatch --jobname "memxct-multinode-$d-1" --nodes=1 --ntasks-per-node=120 --output="memxct-multinode-$d-1.out" cpu/multinode.sh "$d" 32 512 8 1
	sbatch --jobname "memxct-multinode-$d-2" --nodes=2 --ntasks-per-node=120 --output="memxct-multinode-$d-2.out" cpu/multinode.sh "$d" 32 512 8 2
	sbatch --jobname "memxct-multinode-$d-4" --nodes=4 --ntasks-per-node=120 --output="memxct-multinode-$d-4.out" cpu/multinode.sh "$d" 32 512 8 4
done
