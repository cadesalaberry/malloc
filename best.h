#define INT_MAX 32767
/**
 * Uses the Best fit algorithm, looking for the smallest free
 * block sufficiently large.
 */
 void * malloc_best(size_t nbytes) {

    Header * p, * prevp;
    size_t nunits, min_size = INT_MAX;
    Header * minp = NULL, * minprevp = NULL;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    
    // Creates the list if it does not exist
    if ((prevp = freep) == NULL) {

        base.h.nxt  = &base;
        freep       = &base;
        prevp       = &base;
        base.h.size = 0;
    }


	// Looks for the smallest block that is large enough to hold nbytes of data.
    for (p = prevp->h.nxt; ; prevp = p, p = p->h.nxt) {

        // Checks if it is big enough 
        if (p->h.size >= nunits) {

			// If it is exactly the same size, just swap
            if (p->h.size == nunits) {

                prevp->h.nxt = p->h.nxt;
                
                freep = prevp;
                
                // returns data part to user
                return p + 1;

            } else {

                if (minp == NULL || p->h.size < min_size) {

                    minp = p;
                    minprevp = prevp;
                    min_size = minp->h.size;
                }
            }
        }

		// wrapped around free list
        if (p == freep) {

            if (minp != NULL) {

                // Fills in the tail end
                minp->h.size    -= nunits;
                minp            += minp->h.size;
                minp->h.size     = nunits;
                
                freep = minprevp;
                
                // returns data part to user
                return minp + 1;
            }
            
			 // Checks if we reached the end of the list
            if ((p = more_mem(nunits)) == NULL) {
                // No space left in memory.
                return NULL;
            }
        }
    }
}
