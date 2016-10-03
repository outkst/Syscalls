#include <stdio.h>		/* DEBUG */

/* 
	Linux Syscalls: Semaphore Implementation

	Joe Meszar (jwm54@pitt.edu)
	CS1550 Project 2 (FALL 2016)

	REFERENCES
	----------

*/

/*
[ ] Create a cs1550_sem semaphore data type:
	[ ] Include a 'value' integer data type

	[ ] DOWN() which potentially needs to put the current process to sleep; use a two-step process:
		[ ] Mark the task as not ready (but can be awoken by signals):
			- set_current_state(TASK_INTERRUPTIBLE);
		[ ] Invoke the scheduler to pick a ready task:
			- schedule();

	[ ] UP() which potentially needs to wake up a sleeping process. You can do this via:
		[ ] wake_up_process(sleeping_task);
			[ ] sleeping_task is a `struct task_struct` that represents a process put to sleep in `down()`
			[ ] Get the current process's `task_struct` by accessing the global variable `current`. 
			[ ] Store the You may need to save these someplace.

[ ] Make two new system calls:
	[ ] asmlinkage long sys_cs1550_down(struct cs1550_sem *sem)
	[ ] asmlinkage long sys_cs1550_up(struct cs1550_sem *sem)

ADDING A NEW SYSCALL

	[ ] linux-2.6.23.1/kernel/sys.c
		- This file contains the actual implementation of the system calls.

	[ ] linux-2.6.23.1/arch/i386/kernel/syscall_table.S
		- This file declares the number that corresponds to the syscalls
		
	[ ] linux-2.6.23.1/include/asm/unistd.h
		- This file exposes the syscall number to C programs which wish to use it.
*/

struct cs1550_sem
{
   int value;
   //Some process queue of your devising
};

asmlinkage long sys_cs1550_down(struct cs1550_sem *sem);
asmlinkage long sys_cs1550_up(struct cs1550_sem *sem);