echo -e "Grabbing kernel 'bzImage' from thoth.cs.pitt.edu ..."
echo -e "Grabbing system call map 'System.map' from thoth.cs.pitt.edu ..."
echo -e "Grabbing test file 'test.c' from thoth.cs.pitt.edu ...\n"

scp jwm54@thoth.cs.pitt.edu:/afs/pitt.edu/home/j/w/jwm54/private/cs1550/project2//\{test,linux-2.6.23.1/arch/i386/boot/bzImage,linux-2.6.23.1/System.map\} .

echo -e "\nCopying 'bzImage' to /boot/bzImage-devel"
echo -e "Copying 'System.map' to /boot/System.map-devel\n"

cp bzImage /boot/bzImage-devel
cp System.map /boot/System.map-devel

echo -e "Copy Successful!\n"

echo -e "Updating the bootloader with 'lilo' command\n"

lilo