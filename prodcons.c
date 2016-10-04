#include <stdio.h>	/* DEBUG */
#include <unistd.h> /* custom UP() and DOWN() syscalls */

typedef struct
{
	int value;
} cs1550_sem;

cs1550_sem sem;

void up(cs1550_sem *sem);
void down(cs1550_sem *sem);

int main() {
	sem.value=100;

	printf("sem.value = %d\n", sem.value);

	printf("calling UP() with value=%d\n", sem.value);
	up(&sem);
	printf("UP() returned sem.value+1? %d\n", sem.value);

	printf("calling __NR_cs1550_down with test=%d\n", sem.value);
	down(&sem);
	printf("DOWN() returned sem.value+1? %d\n", sem.value);

	return 0;
}

void up(cs1550_sem *sem) {
	int i = syscall(__NR_cs1550_up, sem);
	printf("\tsyscall returned: %d\n", i);
}

void down(cs1550_sem *sem) {
	int i = syscall(__NR_cs1550_down, sem);
	printf("\tsyscall returned: %d\n", i);
}