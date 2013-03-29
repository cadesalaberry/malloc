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

	if (current_policy == 1) {
		return malloc_first(nbytes);
	
	} else if (current_policy == 2) {
		return malloc_best(nbytes);
	
	} else {
		exit(1);
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

/**
 * Changes size of the memory block and returns address to the new one.
 */
void * realloc(void * ptr, size_t size) {
	
    Header *bp, *p;

    /*
     * Take care of some simple, basic cases like when the given pointer is
     * NULL or the size is non-positive.
     */

    if (ptr == NULL) {
        return malloc(size);
    } else if (size <= 0) {
        free(ptr);
        return NULL;
    }

    bp = (Header*) ptr - 1;
    unsigned int numbytes = sizeof(Header) * (bp->s.size - 1);

	// Does not resize if not needed.
    if (size == numbytes) {
        return ptr;
    }


    /*
     * Simply allocate as much memory as the new size and copy the old data
     * into the new place. Then free the previously allocated memory.
     */
     
    p = malloc(size);
    
    if (p == NULL) {
        return NULL;
    }

    if (size < numbytes) {
        numbytes = size;
    }

    memcpy(p, ptr, numbytes);
    
    free(ptr);

    return p;
}

void my_mallinfo() {
	
}
