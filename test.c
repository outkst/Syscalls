#include <stdio.h>	/* DEBUG */
#include <unistd.h>

int main() {
	long retval;
	retval = syscall(__NR_cs1550_up);
	printf("Tried calling __NR_cs1550_up, result should be 666: %ld\n", retval);

	retval = syscall(__NR_cs1550_down);
	printf("Tried calling __NR_cs1550_down, result should be 999: %ld\n", retval);
	
	return 0;
}