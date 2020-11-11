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

## Visualizing

In order to visualize the reconstructed output from MemXCT, you may use the Fiji
image viewer:

- Select "File"
- Select "Import"
- Select "Raw"
- Select the appropriate `.bin` file, e.g. `recon_CDS2.bin`
- Select "32-bit real" for the image type
- Enter the appropriate dimensions
  - 512x512 for `recon_CDS1.bin`
  - 1024x1024 for `recon_CDS1.bin`
- "Offset to first image" and "Gap between images" should both be 0. "Number of images" should be 1.
- Select "little-endian byte order"
- Deselect "White is zero", "Open all files in folder", and "Use virtual stack".

All other images are simply `.png` files, and may be opened in any image viewer.
For example, `.png` files may be opened in Fiji simply by clicking "File" >
"Open" and selecting the appropriate image file.

