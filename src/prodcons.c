/* 
	Syscalls (Producer/Consumer problem)

	Joe Meszar (jwm54@pitt.edu)
	CS1550 Project 1 (FALL 2016)

	Skeleton code for prodcons problem
	(C) Mohammad H. Mofrad, 2016
 */
#include <linux/prodcons.h>		/* custom UP() and DOWN() syscalls */
#include <stdio.h>
#include <stdlib.h>				/* atoi() */
#include <string.h>
#include <time.h>

/*
	Avoid having unwanted forks
	when you call fork, it starts where the PC left off.
	might end up have double or triple processes
	come up with idea to store PID of each and check against new forked program
		if its a newer process, then

	PID for parent process 
	if forked from parent, your PID inside is ZERO but parent holds parent
	- store PID in parent process
	- child, access the shared memory to get the PID


	syscalls.h
	#include <linux/prodcons
	struct cs1550_sem
	asmlinkage long sys_call


	Implmement the IPC part of Mohammeds code
	Otherwise, can test everything except IPC on thoth
		logic for fork
		logic for consumer

	The queue should be of type TASK_STRUCT
		can be array or linked list, doesn't matter

	down(sem s) {
		s.value -=1;
		if (value < 0) {
			//add this process to s.queue
			s.enqueue(currentProcess);
			sleep();
		}
	}

	up
*/

void *MEM_BASE_PTR;
void *MEM_EMPTY;
void *MEM_FULL;
void *MEM_MUTEX;
void *MEM_BUF;

void consumer();
void cs1550_up(struct cs1550_sem *sem);
void cs1550_down(struct cs1550_sem *sem);
void print_stuff(int value, int *MEM_BASE_PTR, int *curr_ptr, int *new_ptr);
void producer();


/* 
	argc 	holds number of arguments
	argv	holds the value of each argument
*/
void main(int argc, char *argv[])
{
	int num_prods;			// number of producers (chefs)
	int num_cons;			// number of consumers (customers)
	int size_buffer;		// buffer size (value)

	// branch between Owner and Children
	//int pid = fork();

	// get the user input
	// if (argc != 4) {
	// 	printf("Please provide correct parameters: \n\n\t./prodcons [numprods] [numcons] [numresources]");
	// 	exit(1);

	// } else {
	// 	num_prods = atoi(argv[1]);
	// 	num_cons = atoi(argv[2]);
	// 	size_buffer = atoi(argv[3]);
	// }

	num_prods = 5;
	num_cons = 5;
	size_buffer = 100;

	/* 
		Begin by creating a SHARED Read/Write memory map to be used across fork()'s.

		3x semaphores	(empty, full, and mutex)
		2x ints			(current producer place, current consumer place)
		1x buffer		()
	*/
	int MAPSIZE = ((sizeof(struct cs1550_sem)) * 3 * (sizeof(int)) * 2 * (sizeof(int)*size_buffer));
	MEM_BASE_PTR = (void *) mmap(NULL, MAPSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);

	if(MEM_BASE_PTR == (void *) -1) 
	{
		fprintf(stderr, "Error mapping memory\n");
		exit(1);
	}

	/* Good to go, initialize pointers to this newly mapped, shared space */
	int *base_ptr = MEM_BASE_PTR;
	int *new_ptr;
	int *curr_ptr = base_ptr;
	int sem_size = sizeof(struct cs1550_sem);
	curr_ptr = curr_ptr + sem_size;
	if(curr_ptr > base_ptr + MAPSIZE) 
	{
		fprintf(stderr, "Address out of range\n");
		exit(1);
	}
	else
	{
		new_ptr = curr_ptr - sem_size;
	}

	MEM_EMPTY = base_ptr;				// EMPTY sem at first location of mapped memory
	MEM_FULL = base_ptr + sem_size;		// FULL sem at second location of mapped memory
	MEM_MUTEX = MEM_FULL + sem_size;	// MUTEX sem at third location of mapped memory
	MEM_BUF = MEM_MUTEX + sem_size;		// BUFFER at final location of mapped memory

	struct cs1550_sem *sem_empty = (struct cs1550_sem *) MEM_EMPTY;
	struct cs1550_sem *sem_full = (struct cs1550_sem *) MEM_FULL;
	struct cs1550_sem *sem_mutex = (struct cs1550_sem *) MEM_MUTEX;

	sem_empty->value = size_buffer;		// value is equal to max resources (size of buffer)
	sem_full->value = 0;				// no item is considered full in the beginning
	sem_mutex->value = 1;				// mutex set

	/* Create X number of producers */
	int i, j;
	i = 0;
	j = 0;
	printf("\nEntering producer fork loop\n");
	for (i=0; i<num_prods; i++) {
		printf("prods called: %d\n", i);
		
		if (fork() == 0) {							// 0 means its a child process, not parent (good!)
			printf("forked producer child\n");
			producer();								// use method w/infinite loop (no return)
		}
	}


	/* Create X number of consumers */
	printf("\nEntering consumer fork loop\n");
	for (j=0; j<num_cons; j++) {
		printf("cons called: %d\n", j);

		if (fork() == 0) {							// 0 means its a child process, not parent (good!)
			printf("forked producer child\n");
			consumer();								// use method w/infinite loop (no return)
		}
	}

	printf("we are now going to loop indefinitely");
	while (1) {
		// stay alive so we don't have zombie processes (where pid=NULL)
	}

	// print_stuff(sem->value, base_ptr, curr_ptr, new_ptr);

	// printf("** calling cs1550_down... **\n");
	// cs1550_down(sem);
	// printf("\n");

	// sleep(2);

	// print_stuff(sem->value, base_ptr, curr_ptr, new_ptr);

	// printf("** calling cs1550_up... **\n");
	// cs1550_up(sem);

	// sleep(2);

	// print_stuff(sem->value, base_ptr, curr_ptr, new_ptr);
}

/* Make those marfkin pancakes */
void producer() {
	// int in = 0;
	// int pitem;

	// /* produce an item into pitem */
	// while (1) {
	// 	sleep(2);				// DEBUG

	// 	empty.down();			// attempt to produce one more item (one less empty)

	// 	/* ENTER CRITICAL REGION */
	// 	mutex.down();			// LOCK

	// 	buffer[in] = pitem;
	// 	in = (in+1) % n;		// circular reference

	// 	mutex.up();				// UNLOCK
	// 	/* EXIT CRITICAL REGION */

	// 	full.up();				// successfully produced one item (one more full)
	// }
}

/* Eat those delicious marfkin pancakes */
void consumer() {
	// int out = 0;
	// int pitem;

	// /* consume an item from citem */
	// while (1) {
	// 	sleep(2);				// DEBUG

	// 	full.down();			// attempt to consume one more item (one less full)

	// 	/* ENTER CRITICAL REGION */
	// 	mutex.down();			// LOCK

	// 	citem = buffer[out];
	// 	out = (out+1) % n;

	// 	mutex.up();				// UNLOCK
	// 	/* EXIT CRITICAL REGION */

	// 	empty.up();
	// }
}

void print_stuff(int value, int *base_ptr, int *curr_ptr, int *new_ptr) {
	printf("SEMAPHORE VALUE: (%d)\n", value);
	printf("BASEPTR-ADDR (0x%08x), CURPTR-ADDR (0x%08x), NEWPTR-ADDR (0x%08x)\n", base_ptr, curr_ptr, new_ptr);
	printf("BASEPTR (%d), CURPTR (%d), NEWPTR (%d)\n\n", *base_ptr, *curr_ptr, *new_ptr);
}

void cs1550_down(struct cs1550_sem *sem) 
{
	int i;
	if (i = syscall(__NR_sys_cs1550_down, sem) < 0) {
		printf("ERROR (%d) RETURNED FROM __NR_sys_cs1550_down", i);
	}
}

void cs1550_up(struct cs1550_sem *sem) 
{
	int i;
	if (i = syscall(__NR_sys_cs1550_up, sem) < 0) {
		printf("ERROR (%d) RETURNED FROM __NR_sys_cs1550_up", i);
	}
}