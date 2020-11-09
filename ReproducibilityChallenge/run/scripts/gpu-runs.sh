#!/bin/bash

for f in gpu/*; do
	qsub "$f"
done

