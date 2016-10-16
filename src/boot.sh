#!/bin/bash
# Fast boot script for rebuilding Linux kernel
# (C) Mohammad H. Mofrad, 2016

# Change the directory based on your working directory on thoth
scp jwm54@thoth.cs.pitt.edu:/u/OSLab/jwm54/\{bzImage,System.map,prodcons\} ./
# Copy kernel files
/bin/cp -rf bzImage /boot/bzImage-devel
/bin/cp -rf System.map /boot/System.map-devel
# Boot from modified kernel
lilo
