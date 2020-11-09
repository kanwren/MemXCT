#!/bin/bash

for f in cpu/*; do
	qsub "$f"
done

