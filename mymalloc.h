#include "header.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



static int current_policy = 1;

// Import the different strategies.
#include "first.h"
#include "best.h"

// We assume you have defined the following two definitions
// If so, you should remove these..
// If not, move them to your mymalloc.h file
#define FIRST_FIT                         1
#define BEST_FIT                          2

void *  my_malloc   (size_t nbytes);
void    my_free     (void * data_pointer);
void    my_mallopt  (int	policy);
void    my_mallinfo ();

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
 * Releases the block and put it in the free list.
 */
 void my_free(void * data_pointer) {

    if (data_pointer == NULL) {
        return;
    }

    Header * tofree, * p;
    
    // Gets the header of the given data pointer
    tofree = (Header *) data_pointer - 1;

    if (tofree->h.size <= 0) {
        return;
    }

    // Looks for the closest free header
    for (p = freep; !(tofree > p && tofree < p->h.nxt); p = p->h.nxt) {
        if (p >= p->h.nxt && (tofree > p || tofree < p->h.nxt)) {
            break;
        }
    }


    if (tofree + tofree->h.size == p->h.nxt) {
        // joins to upper block
        tofree->h.size += p->h.nxt->h.size;
        tofree->h.nxt = p->h.nxt->h.nxt;
    } else {
        tofree->h.nxt = p->h.nxt;
    }
    
    if (p + p->h.size == tofree) {
        // joins to lower block
        p->h.size += tofree->h.size;
        p->h.nxt = tofree->h.nxt;
    } else {
        p->h.nxt = tofree;
    }
    
    freep = p;
}

void my_mallinfo() {
	
}

/**
 * gets more memory from the system and returns pointer to it.
 */
 static Header * morecore(unsigned int nu) {

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
    up->h.size = nu;
    
    my_free((void *) (up + 1));
    
    return freep;
}
