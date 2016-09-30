# SYSCALLS
Solve the Producer/Consumer Parallelism problem implementing a custom semaphore mutex data structure.

This solution will implement the custom semaphore at the kernel-level, to avoid the
problem of context-switching from hardware interrupts (pre-emption). The pre-emption
can only be disabled within the kernel-space as opposed to the user-space: this is
because it takes elevated privileges to tell the hardware not to send interrupts.

With interrupts disabled, this code will allow our custom semaphore to issue DOWN() and
UP() events with regards to a buffer of items.

##SEMAPHORE WITH BLOCKING
```
class Semaphore {
    int value;
    ProcessList pl;

    void down() {
        value -= 1;
        if (value < 0) {                    // add this process to process list
            pl.enqueue(currentProcess);
            Sleep();                        // Put this process/thread to sleep
        }
    }

    void up() {
        Process P;
        value +=1;
        if (value <= 0) {                   // remove a process P from the process list
            P = pl.dequeue();
            Wakeup(P);                      // allow process/thread P that was sleeping/waiting to run
        }
    }
}
```
