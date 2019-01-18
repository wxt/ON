/*
 * author: iancain
 * date: 10/1/2018
 * description:
 * Objective: speed operations of malloc/free and adapt idiomatically and separate memory
             management from other data storage patterns such as linked lists, stacks,
             double buffering
   Limitations: Fixed sized memory blocks. Due to the O(1) requirement only fixed sized
                memory allocation can be performed. Memory fragmentation and
                collection/collating operations are not desired due to performance demands

  Support O(1) operation in acquire and release operations
  Strategy:
    stack object to manage memory blocks
       acquire = pop_front  (acquire block off the front/top of stack)
       release = push_back  (release block by putting on back/bottom of stack)
 */


#include <stdlib.h>
#include <stdbool.h>   // NOTE: c99 bool requires #include <stdbool.h>

typedef struct memory_pool memory_pool_t;

memory_pool_t * memory_pool_init(size_t count, size_t block_size);
bool memory_pool_destroy(memory_pool_t *mp);

void * memory_pool_acquire(memory_pool_t *mp);
bool memory_pool_release(memory_pool_t *mp, void * data);

// convieneince functions
size_t memory_pool_available(memory_pool_t *mp);
void memory_pool_dump(memory_pool_t *mp);
