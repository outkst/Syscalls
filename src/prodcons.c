/* 
    Syscalls (Producer/Consumer problem)

    Joe Meszar (jwm54@pitt.edu)
    CS1550 Project 2 (FALL 2016)
 */
#include <linux/prodcons.h>     /* custom UP() and DOWN() syscalls */
#include <stdio.h>              /* printf() */
#include <stdlib.h>             /* atoi() */
#include <string.h>
#include <time.h>

void *MEM_BASE_PTR;
void *MEM_EMPTY;
void *MEM_FULL;
void *MEM_MUTEX;
int *MEM_CUR_PROD;
int *MEM_CUR_CON;
int *MEM_BUF;

int *in;
int *out;
int resources;
struct cs1550_sem *sem_empty;
struct cs1550_sem *sem_full;
struct cs1550_sem *sem_mutex;

void consumer(int fork_num);
void producer(int fork_num);
void cs1550_up(struct cs1550_sem *sem);
void cs1550_down(struct cs1550_sem *sem);

/* 
    argc    holds number of arguments
    argv    holds the value of each argument
*/
int main(int argc, char *argv[])
{
    int num_prods;          // number of producers (chefs)
    int num_cons;           // number of consumers (customers)
    int size_buffer;        // buffer size (value)

    // get the user input for Producers Consumers Resources
    if (argc != 4) {
        printf("Please provide correct parameters: \n\n\t./prodcons [numprods] [numcons] [numresources]\n");
        exit(1);

    } else {
        num_prods = atoi(argv[1]);
        num_cons = atoi(argv[2]);
        size_buffer = atoi(argv[3]);
    }

    /* 
        Begin by creating a SHARED Read/Write memory map to be used across fork()'s.

        3x semaphores       (empty, full, and mutex)
        2x ints             (current producer place, current consumer place)
        1x buffer           (the number of (int) resources)
    */
    int MAPSIZE = ((sizeof(struct cs1550_sem)) * 3 * (sizeof(int)) * 2 * (sizeof(int)*size_buffer));
    MEM_BASE_PTR = (void *) mmap(NULL, MAPSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);

    if(MEM_BASE_PTR == (void *) -1)
    {
        fprintf(stderr, "Error mapping memory\n");
        exit(1);
    }

    /* 
        Good to go, initialize pointers to this 
        newly mapped, shared space.
    */
    int sem_size = sizeof(struct cs1550_sem);       // get the size of a semaphore struct
    MEM_EMPTY = MEM_BASE_PTR;                       // EMPTY sem at first location of mapped memory
    MEM_FULL = MEM_BASE_PTR + sem_size;             // FULL sem at second location of mapped memory
    MEM_MUTEX = MEM_FULL + sem_size;                // MUTEX sem at third location of mapped memory
    MEM_CUR_PROD = MEM_MUTEX + sem_size;            // CURRENT PRODUCER resource number
    MEM_CUR_CON = MEM_CUR_PROD + sizeof(int);       // CURRENT CONSUMER resource number
    MEM_BUF = MEM_MUTEX + sem_size;                 // BUFFER at sixth (final) location of mapped memory

    sem_empty = (struct cs1550_sem *) MEM_EMPTY;    // make MEM_EMPTY reference into cs1550_sem reference
    sem_full = (struct cs1550_sem *) MEM_FULL;      // make MEM_FULL reference into cs1550_sem reference
    sem_mutex = (struct cs1550_sem *) MEM_MUTEX;    // make MEM_MUTEX reference into cs1550_sem reference

    in = MEM_CUR_PROD;                              // make MEM_CUR_PROD reference into (int) reference
    out = MEM_CUR_CON;                              // make MEM_CUR_CON reference into (int) reference
    resources = size_buffer;                        // global shared resources equal to user-input buffer size

    sem_empty->value = size_buffer;                 // value is equal to max resources (size of buffer)
    sem_full->value = 0;                            // no item is considered full in the beginning
    sem_mutex->value = 1;                           // mutex set to '1' so that one process can successfully run. the rest are queued.

    /* Create X number of producers */
    int i, j;
    i = 0;
    j = 0;
    for (i=0; i<num_prods; i++) {
        if (fork() == 0) {                          // 0 means its a child process, not parent (good!)
            producer(i);                            // use method w/infinite loop (no return)
        }
    }

    /* Create X number of consumers */
    for (j=0; j<num_cons; j++) {
        if (fork() == 0) {                          // 0 means its a child process, not parent (good!)
            consumer(j);                            // use method w/infinite loop (no return)
        }
    }

    while (1) {
        /* stay alive to avoid zombie processes */
    }

    return 0;
}

/* Make those marfkin pancakes */
void producer(int fork_num) {

    /* produce items into the buffer */
    while (1) {
        cs1550_down(sem_empty);                     // attempt to produce one more item (one less empty)

        /* ENTER CRITICAL REGION */
        cs1550_down(sem_mutex);                     // LOCK

        MEM_BUF[*in] = *in;                         // store resource number to show that something has been produced
        *in = ((*in)+1) % resources;                // increment to next resource (with wrapping to beginning)

        printf("CHEF #%d PRODUCED PANCAKE #%d\n", fork_num, *in);    /* !! DEBUG !! */
        
        cs1550_up(sem_mutex);                       // UNLOCK
        /* EXIT CRITICAL REGION */

        cs1550_up(sem_full);                        // reflect that one item was produced (one more full)
    }
}

/* Eat those delicious marfkin pancakes */
void consumer(int fork_num) {
    int citem;

    /* consume items from the buffer */
    while (1) {
        cs1550_down(sem_full);                      // attempt to consume one more item (one less full)

        /* ENTER CRITICAL REGION */
        cs1550_down(sem_mutex);                     // LOCK

        citem = MEM_BUF[*out];                      // get the resource out
        //MEM_BUF[*out] = -1;                       // (DEBUG) Show that this has been consumed by inserting '-1'
        *out = ((*out)+1) % resources;              // increment to next resource (with wrapping to beginning)

        printf("CUSTOMER #%d ATE PANCAKE #%d\n", fork_num, *out);    /* !! DEBUG !! */

        cs1550_up(sem_mutex);                       // UNLOCK
        /* EXIT CRITICAL REGION */

        cs1550_up(sem_empty);                       // reflect that one item was consumed (one more empty)
    }
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
