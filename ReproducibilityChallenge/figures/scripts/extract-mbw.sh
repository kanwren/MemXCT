#!/bin/bash

grep -oP '(?<=tot: )\d*\.\d*' "$1"
