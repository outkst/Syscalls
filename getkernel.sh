echo "Grabbing kernel 'bzImage' from thoth.cs.pitt.edu ..."
echo "Grabbing system call map 'System.map' from thoth.cs.pitt.edu ..."
echo "Grabbing test file 'test.c' from thoth.cs.pitt.edu ..."

scp jwm54@thoth.cs.pitt.edu:/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2//\{test,linux-2.6.23.1/arch/i386/boot/bzImage,linux-2.6.23.1/System.map\} .
