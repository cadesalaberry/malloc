#include "header.h"
//#include "brk.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static Header base;

static Header * freep = NULL;

static int current_policy = 1;

// Import the different strategies.
#include "first.h"
#include "best.h"

void * my_malloc(size_t nbytes);
void my_free(void * ptr);
void my_mallopt(int	policy);
void my_free(void * ap);
void * realloc(void * ptr, size_t size);
void my_mallinfo();




/**
 * Gets the address of the start of the allocated memory.
 * choses the method according to the policy specified.
 */
void * my_malloc(size_t nbytes) {
	
    if (nbytes == 0) {
        return NULL;
    }

    void * to_return;

	if (current_policy == 1) {
		to_return = malloc_first(nbytes);
	
	} else if (current_policy == 2) {
		to_return = malloc_best(nbytes);
	
	}

    if (to_return == NULL) {
        extern char * my_malloc_error;
        exit(1);
    }
    
    return to_return;
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
            break; /* freed block at start or end of arena */
		}
	}


    if (bp + bp->s.size == p->s.ptr) {
        // joins to upper nbr
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
	}
	
    if (p + p->s.size == bp) {
        // joins to lower nbr
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
	}
	
    freep = p;
}

void my_mallinfo() {
	
}
