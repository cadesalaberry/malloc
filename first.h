/**
 * Gets the start address of the newly allocated memory.
 * Gets the first free memory that is large enough.
 */
void * malloc_first(size_t nbytes) {
	
    Header * p, * prevp;
    size_t nunits;

    nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
    
    // Creates the list if it does not exist
    if ((prevp = freep) == NULL) {

        base.h.nxt  = &base;
        freep       = &base;
        prevp       = &base;
        base.h.size = 0;
    }


    // Gets a block large enough to hold nbytes of data.
    for (p = prevp->h.nxt; ; prevp = p, p = p->h.nxt) {
		
		// Checks if it is big enough
        if (p->h.size >= nunits) {
			
			// Checks if it is the same size
            if (p->h.size == nunits) {
                prevp->h.nxt = p->h.nxt;
            }
            else {
                // Fills in tail end
                p->h.size   -= nunits;
                p           += p->h.size;
                p->h.size    = nunits;
            }

            freep = prevp;
            
            // returns data part of block to user
            return p + 1; 
        }
        
        // Wrapped around free list
        if (p == freep) {
			
			// Gets another free header
            if ((p = morecore(nunits)) == NULL) {
                // No space left in memory.
                return NULL;
            }
        }
    }
}
