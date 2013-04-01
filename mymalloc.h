#include "header.h"
//#include "brk.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// We assume you have defined the following two definitions
// If so, you should remove these..
// If not, move them to your mymalloc.h file
#define FIRST_FIT                         1
#define BEST_FIT                          2

static Header base;
static Header * freep = NULL;
static int current_policy = BEST_FIT;

// Import the different strategies.
#include "first.h"
#include "best.h"

void * my_malloc(size_t nbytes);
void my_free(void * ptr);
void my_mallopt(int	policy);
void my_free(void * ap);
void my_mallinfo();

/**
 * Gets the address of the start of the allocated memory.
 * choses the method according to the policy specified.
 */
void * my_malloc(size_t nbytes) {
	
    if (nbytes == 0) {
        return NULL;
    }

    void * toReturn;

	if (current_policy == FIRST_FIT) {
		toReturn = malloc_first(nbytes);
	
	} else if (current_policy == BEST_FIT) {
		toReturn = malloc_best(nbytes);
	}

    if (toReturn == NULL) {
        extern char * my_malloc_error;
    }
}

void my_mallopt(int	policy) {
	
	current_policy = policy;
	
}

/**
 * gets more memory from the system and returns pointer to it.
 */
static Header * morecore(unsigned nu) {

    char * cp;
    Header * up;
    
    if (nu < NALLOC) {
        nu = NALLOC;
	}
    cp = sbrk(nu * sizeof(Header));
	
	// Checks if there is no space at all
    if (cp == (char *) - 1) {
        return NULL;
	}
	
    up = (Header *) cp;
    up->s.size = nu;
    
    my_free((void *) (up + 1));
    
    return freep;
}

/**
 * Releases the block and put it in the free list.
 */
void my_free(void * ap) {

    if (ap == NULL) {
        return;
    }

    Header * bp, * p;
    bp = (Header *) ap - 1;

    if (bp->s.size <= 0) {
        return;
    }

    /*
     * Merges the freed block with adjacent free memory.
     */

    // Points to block header
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) {
            break;
		}
	}

    if (bp + bp->s.size == p->s.ptr) {
        // joins to upper block
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
	}
	
    if (p + p->s.size == bp) {
        // joins to lower block
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
	}
	
    freep = p;
}

void my_mallinfo() {
	
}
