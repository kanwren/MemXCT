#!/bin/bash

for i in {6..10}; do
	for n in 1 2 4; do
		sbatch --job-name "memxct-singlenode-gpu-CDS1_${i}.out" --nodes 1 --ntasks-per-node 24 --output "memxct-singlenode-gpu-CDS1_${i}.out" gpu/singlenode.sh "$d" 128 512 48
		sbatch --job-name "memxct-singlenode-gpu-CDS2_${i}.out" --nodes 1 --ntasks-per-node 24 --output "memxct-singlenode-gpu-CDS2_${i}.out" gpu/singlenode.sh "$d" 128 512 48
	done
done

for i in {1..10}; do
	for d in CDS1 CDS2; do
		sbatch --job-name "memxct-multinode-gpu-${d}-1_${i}.out" --nodes 1 --ntasks-per-node 24 --output "memxct-multinode-gpu-${d}-1_${i}.out" gpu/multinode.sh "$d" 128 512 48
		sbatch --job-name "memxct-multinode-gpu-${d}-2_${i}.out" --nodes 2 --ntasks-per-node 24 --output "memxct-multinode-gpu-${d}-2_${i}.out" gpu/multinode.sh "$d" 128 512 48
		sbatch --job-name "memxct-multinode-gpu-${d}-4_${i}.out" --nodes 4 --ntasks-per-node 24 --output "memxct-multinode-gpu-${d}-4_${i}.out" gpu/multinode.sh "$d" 128 512 48
	done
done

