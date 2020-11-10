#!/bin/bash

for i in {1..10}; do
	for d in CDS1 CDS2 CDS3; do
		sbatch --job-name "memxct-singlenode-${d}_${i}" --nodes=1 --ntasks-per-node=60 --output="memxct-singlenode-${d}_${i}.out" cpu/singlenode.sh "$d" 32 512 8
	done
done

for i in {1..10}; do
	for d in CDS1 CDS2; do
		sbatch --job-name "memxct-multinode-${d}-1_${i}" --nodes=1 --ntasks-per-node=120 --output="memxct-multinode-${d}-1_${i}.out" cpu/multinode.sh "$d" 32 512 8
		sbatch --job-name "memxct-multinode-${d}-2_${i}" --nodes=2 --ntasks-per-node=120 --output="memxct-multinode-${d}-2_${i}.out" cpu/multinode.sh "$d" 32 512 8
		sbatch --job-name "memxct-multinode-${d}-4_${i}" --nodes=4 --ntasks-per-node=120 --output="memxct-multinode-${d}-4_${i}.out" cpu/multinode.sh "$d" 32 512 8
	done
done

