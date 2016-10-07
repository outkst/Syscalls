#!/bin/bash
echo -e "Copying implementation of syscalls 'sys.c' to linux-2.6.23.1/kernel/sys.c ..."
echo -e "Copying syscall number file 'syscall_table.S' to linux-2.6.23.1/arch/i386/kernel/syscall_table.S ..."
echo -e "Copying syscall library exposing file 'unistd.h' to linux-2.6.23.1/include/asm/unistd.home ...\n"

cp ../sys.c /linux-2.6.23.1/kernel/
cp ../syscall_table.S /linux-2.6.23.1/arch/i386/kernel/
cp ../unistd.h /linux-2.6.23.1/include/asm/

echo -e "Making new kernel with new syscalls ..."

make ARCH=i386 bzImage 
