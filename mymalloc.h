#include "header.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Import the different strategies.
#include "first.h"
#include "best.h"

// We assume you have defined the following two definitions
// If so, you should remove these..
// If not, move them to your mymalloc.h file
#define FIRST_FIT                   1
#define BEST_FIT                    2

static int current_policy = FIRST_FIT;
char * my_malloc_error;

void *  my_malloc   (size_t   nbytes);
void    my_free     (void   * data_pointer);
void    my_mallopt  (int	  policy);
void    dissolve    (Header * to_dis);
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

    // Gets the header of the given data pointer
    Header * tofree = (Header *) data_pointer - 1;

    if (tofree->h.size <= 0) {
        return;
    }

    dissolve(tofree);
}

/**
 * Merges the block with the closest free blocks.
 */
void dissolve(Header * to_dis) {

    Header * p;

    // Looks for the closest free header
    for (p = freep; !(to_dis > p && to_dis < p->h.nxt); p = p->h.nxt) {
        if (p >= p->h.nxt && (to_dis > p || to_dis < p->h.nxt)) {
            break;
        }
    }


    if (to_dis + to_dis->h.size == p->h.nxt) {
        // joins to upper block
        to_dis->h.size += p->h.nxt->h.size;
        to_dis->h.nxt = p->h.nxt->h.nxt;
    } else {
        to_dis->h.nxt = p->h.nxt;
    }

    if (p + p->h.size == to_dis) {
        // joins to lower block
        p->h.size += to_dis->h.size;
        p->h.nxt = to_dis->h.nxt;
    } else {
        p->h.nxt = to_dis;
    }

    freep = p;

    // Checks if the last block is greater than 128kb
    if (freep->h.size <= 131072 && freep->h.nxt == &base) {
        freep->h.size = 131072;
        brk(freep + 131072);
    }
}
void my_mallinfo() {
	
    Header * p;

    int nb_blocks = 0;
    size_t total = 0;

    // Gathers stats
    for (p = base.h.nxt; p->h.nxt != &base; p = p->h.nxt) {
        nb_blocks++;
        total += p->h.size;
    }

    printf("Free blocks\t: %i\n", nb_blocks);
    printf("Free space \t: %i\n", total);
    printf("Memory span\t: %i\n", p+p->h.size-&base);
}

/**
 * gets more memory from the system and returns pointer to it.
 */
 static Header * more_mem(size_t asked) {

    char * cp;
    Header * up;
    
    if (asked < NALLOC) {
        asked = NALLOC;
    }
    cp = sbrk(asked * sizeof(Header));
    
    // Checks if there is no space at all
    if (cp == (char *) - 1) {
        return NULL;
    }
    
    up = (Header *) cp;
    up->h.size = asked;
    
    my_free((void *) (up + 1));
    
    return freep;
}
