#include <stdio.h>	/* DEBUG */
#include <unistd.h> /* custom UP() and DOWN() syscalls */

struct cs1550_sem
{
	int value;
};

void up(struct cs1550_sem *sem);
void down(struct cs1550_sem *sem);

struct cs1550_sem sem;

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

void up(struct cs1550_sem *sem) {
	int i = syscall(__NR_cs1550_up, sem);
	printf("\tsyscall returned: %d\n", i);
}

void down(struct cs1550_sem *sem) {
	int i = syscall(__NR_cs1550_down, sem);
	printf("\tsyscall returned: %d\n", i);
}