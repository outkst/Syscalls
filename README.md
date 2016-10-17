# SYSCALLS OR: HOW I LEARNED TO SOLVE THE PRODUCER/CONSUMER PROBLEM
Solve the Producer/Consumer Parallelism problem implementing a custom semaphore mutex data structure.

This solution will implement the custom semaphore at the kernel-level, to avoid the
problem of context-switching from hardware interrupts (pre-emption). The pre-emption
can only be disabled within the kernel-space as opposed to the user-space: this is
because it takes elevated privileges to tell the hardware not to send interrupts.

With interrupts disabled, this code will allow our custom semaphore to issue `DOWN()` and
`UP()` events with regards to a buffer of items. The `DOWN()` and `UP()` syscalls will atomically
decrement and increment a given "resource", respectively. This "resource" is an abstract int that
corresponds to whatever the semaphore is built to handle (In this instance, it will be Pancakes).

To properly implement this pseudoparallelism, three semaphores will be used:
* `EMPTY` - This semaphore will hold the number of "empty" resources; initialized to the number of maximum resources (buffer size). In laymens, "Initially, all possible resources have yet to be produced".  
* `FULL` - Holds the number of "full" resources; initialized to zero. In laymens, "There are initially zero resources produced."  
* `MUTEX` - "Binary" value determining if a process is currently in the Critical Region of code. "Binary" in quotes because it is initialized to `1`, and is decremented whenever a process enters the Critical Region. All other processes that attempt to access the CR will further decrement the value (into negative), and be inserted into a queue of processes waiting to enter the Critical Region of code. The value `1` means in English, "There is only one resource--the _Critical Region_".  

Queueing processes is performed using the custom `proc_node` linked-list struct, outlined in `prodcons.h`. 
The semaphore will hold a reference to the HEAD and TAIL of this linked-list, and perform First-In-First-Out 
(FIFO) operations by enqueueing into the TAIL, and dequeueing from the HEAD. A semaphore's `value` property
will reflect there exists items in the queue whenever `value < 0`.

This implementation uses the kernel-defined `spin_lock` structure to achieve atomicity. It is defined as "_a low-level, mutual-exclusion synchronization primitive that spins while it waits to acquire a lock_".

##SEMAPHORE WITH BLOCKING (JAVA Pseudocode)
```
class Semaphore {
    int value;
    ProcessList pl;

    void down(Process P) {
        value -= 1;
        if (value < 0) {                    // add this process to process list
            pl.enqueue(P);
            Sleep();                        // Put this process/thread to sleep
        }
    }

    void up() {
        value +=1;
        if (value <= 0) {                   // remove a process P from the process list
            Process P = pl.dequeue();
            Wakeup(P);                      // allow process/thread P that was sleeping/waiting to run
        }
    }
}
```
