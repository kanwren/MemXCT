#!/bin/bash

grep -oP '(?<=avGFLOPS: )\d*\.\d*' "$1"
