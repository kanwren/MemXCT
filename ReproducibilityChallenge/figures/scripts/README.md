# figures/scripts

Here you can find scripts that generated our various figures. For instructions
on which files contain what and how to run the code, see
ReproducibilityChallenge/doc and refer to the figures section.

## Running

Note that in the interest of time, data was extracted from the output logs
manually, as opposed to writing parsing scripts for automatic extraction. See
the CSVs in this directory for the expected structure of the input files.

To view the figures, open the images within the output directory. To reproduce
these figures yourself, go into the scripts folder and open the ipynb (jupyter
notebook) that corresponds to which figures you want to generate. If you are
trying to reproduce the strong scaling experiments (any of the following:
CPU-cds1, CPU-cds2, GPU-cds1, or GPU-cds2) use the file
`StrongScaling_plots.ipynb`. If attempting to create the performance plots (for
single node CPU or GPU) use `singlenode_performance(gpu_cpu)_plots.ipynb`. I
recommend opening these in google colab, as that will ensure all dependencies
are setup and ready. Additionally, if you wish to generate the memory bandwidth
bar charts, use the code found in `memoryBandwidth_plots.ipynb`. The data inside
these files is populated by hand, but we calculated the numbers by manually
looking at our log files (getting them into csv using vim) and parsing what we
needed from the CSVs. These final CSVs can also be found in our figures/scripts
directory.

