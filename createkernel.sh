echo -e "Copying implementation of syscalls 'sys.c' to linux-2.6.23.1/kernel/sys.c ..."
echo -e "Copying syscall number file 'syscall_table.S' to linux-2.6.23.1/arch/i386/kernel/syscall_table.S ..."
echo -e "Copying syscall library exposing file 'unistd.h' to linux-2.6.23.1/include/asm/unistd.home ...\n"
cp "/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2/sys.c" "/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2/linux-2.6.23.1/kernel/sys.c"
cp "/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2/syscall_table.S" "/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2/linux-2.6.23.1/arch/i386/kernel/syscall_table.S"
cp "/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2/unistd.h" "/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2/linux-2.6.23.1/include/asm/unistd.h"
echo -e "Making new kernel with new syscalls ..."
make ARCH=i386 bzImage