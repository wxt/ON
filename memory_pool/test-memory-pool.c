/*
 * author: iancain
 * date: 10/1/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>

#include "memory_pool.h"

#define MAX_MSG_SIZE 500
#define FLE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_MESSAGE(format, args...) do { \
		char msg[MAX_MSG_SIZE]; \
		sprintf(msg,format,##args);  \
		printf("<%s:%s(%d)> %s\n",FLE,__func__,__LINE__,msg); \
} while(0)


//---
// GLOBALS
//

// Test Memory Pool
// Objective: speed operations of malloc/free and adapt idiomatically and separate memory
//           management from other data storage patterns such as linked lists, stacks,
//           double buffer
// Limitations: Fixed sized memory blocks. Due to the O(1) requirement only fixed sized
//                memory allocation can be performed. Memory fragmentation and
//                collection/collating operations are not desired due to performance demands
//
// Support O(1) operation in acquire and release operations
// Strategy:
// stack object to manage memory blocks
// acquire = pop_front  (acquire block off the front/top of stack)
// release = push_back  (release block by putting on back/bottom of stack)
//
//

int main (int argc, char *argv[])
{
	printf("BEGIN TEST :\n");

    size_t count = 5;
    size_t block_size = 10;
    printf("START: TEST: count=%zu, block_size=%zu\n", count, block_size);

    memory_pool_t * mp1 = memory_pool_init(count, block_size);
    if( mp1 == NULL ) {
        printf("TEST: ERROR: count=%zu, block_size=%zu\n", count, block_size);
        return 1;
    }

    memory_pool_dump(mp1);
    printf("available = %zu\n", memory_pool_available(mp1));
    void * data;
    data = memory_pool_acquire(mp1);
    if( data == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
        return 1;
    }
    else {
        printf("TEST: data = %p\n", data);
        printf("TEST: available = %zu\n", memory_pool_available(mp1));
    }

    char test_data[] = "01234567";
    memcpy(data, test_data, sizeof(test_data) );
    printf("data: '%s'\n", (char*)data);

    memory_pool_dump(mp1);   // dump before
    memory_pool_release(mp1, data);
    memory_pool_release(mp1, data);  // test double release
    memory_pool_dump(mp1);   // dump after

    // test over acquire
    void *d1 = memory_pool_acquire(mp1);   // 1 of 5
    if( d1 == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
    }

    void *d2 = memory_pool_acquire(mp1);   // 2 of 5
    if( d2 == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
    }
    void *d3 = memory_pool_acquire(mp1);   // 3 of 5
    if( d3 == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
    }
    void *d4 = memory_pool_acquire(mp1);   // 4 of 5
    if( d4 == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
    }
    void *d5 = memory_pool_acquire(mp1);   // 5 of 5
    if( d5 == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
    }

    // FAIL CASE
    void *d6 = memory_pool_acquire(mp1);   // 6 of 5 !!!!!!!!!!
    if( d6 == NULL ) {
        printf("TEST: ERROR: acquire failed.\n");
    }

    memory_pool_dump(mp1);

    memory_pool_release(mp1, d1);
    memory_pool_release(mp1, d2);
    memory_pool_release(mp1, d3);
    memory_pool_release(mp1, d4);
    memory_pool_release(mp1, d5);
    memory_pool_release(mp1, d5);  // ERROR: double release

    memory_pool_dump(mp1); // verify all items have been returned

    memory_pool_destroy(mp1);

    printf("\nSTOP\n");
	return 1;
}
