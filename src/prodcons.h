/* 
 * Skeleton code for prodcons implementation
 * (C) Mohammad H. Mofrad, 2016  
 *
 */
#ifndef PRODCONS_H_INCLUDED
#define PRODCONS_H_INCLUDED


#include <asm/errno.h>
#include <asm/unistd.h>
#include <asm/mman.h>
// #include <linux/spinlock_types.h>		/* spinlock_t */
// #include <linux/sched.h>				/* task_struct (line937-1198) */

/* 
	Custom linked-list implementation to hold list of processes
*/
struct proc_node {
	struct task_struct *process;		// holds a reference to a process
	struct proc_node *next;				// holds reference to next process node in linked list
};

/* 
	Custom semaphore structure that holds available resources and a 
	queue of processes waiting to use that resource
*/
struct cs1550_sem {
	int value;							/* holds available */
	struct proc_node *head;				/* start of linked-list of processes waiting */
	struct proc_node *tail;				/* end of linked-list of processes waiting */
};

void  cs1550_down(struct cs1550_sem *);
void  cs1550_up  (struct cs1550_sem *);

#endif