#!/bin/bash

for i in {6..10}; do
	sbatch --job-name "memxct-singlenode-gpu-CDS1_${i}" --nodes=1 --ntasks-per-node=1 --output="memxct-singlenode-gpu-CDS1_${i}.out" gpu/singlenode.sh "CDS1" 128 512 48
	sbatch --job-name "memxct-singlenode-gpu-CDS2_${i}" --nodes=1 --ntasks-per-node=1 --output="memxct-singlenode-gpu-CDS2_${i}.out" gpu/singlenode.sh "CDS2" 128 512 48
	sbatch --job-name "memxct-singlenode-gpu-CDS3_${i}" --nodes=1 --ntasks-per-node=1 --output="memxct-singlenode-gpu-CDS3_${i}.out" gpu/singlenode.sh "CDS3" 128 512 48
done

for i in {1..10}; do
	for d in CDS1 CDS2; do
		sbatch --job-name "memxct-multinode-gpu-${d}-1_${i}" --nodes=1 --ntasks-per-node=1 --output="memxct-multinode-gpu-${d}-1_${i}.out" gpu/multinode.sh "$d" 128 512 48 1
		sbatch --job-name "memxct-multinode-gpu-${d}-2_${i}" --nodes=1 --ntasks-per-node=2 --output="memxct-multinode-gpu-${d}-2_${i}.out" gpu/multinode.sh "$d" 128 512 48 2
		sbatch --job-name "memxct-multinode-gpu-${d}-4_${i}" --nodes=1 --ntasks-per-node=4 --output="memxct-multinode-gpu-${d}-4_${i}.out" gpu/multinode.sh "$d" 128 512 48 4
		sbatch --job-name "memxct-multinode-gpu-${d}-8_${i}" --nodes=1 --ntasks-per-node=8 --output="memxct-multinode-gpu-${d}-8_${i}.out" gpu/multinode.sh "$d" 128 512 48 8
	done
done

