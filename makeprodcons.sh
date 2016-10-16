echo "building prodcons.c file"
gcc -m32 -o prodcons -I ./linux-2.6.23.1/include/ ./src/prodcons.c;