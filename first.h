/**
 * Gets the start address of the newly allocated memory.
 * Gets the first free memory that is large enough.
 */
void * malloc_first(size_t nbytes) {
	
    Header * p, * prevp;
    Header * morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
    if ((prevp = freep) == NULL) {
		
		// no free list exist
        base.h.nxt = freep = prevp = &base;
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
                // Allocates tail end
                p->h.size -= nunits;
                p += p->h.size;
                p->h.size = nunits;
            }
            freep = prevp;
            
            // returns Data part of block to user
            return (void *)(p+1); 
        }
        
        // Wrapped around free list
        if (p == freep) {
			
			// Nothing left in the list
            if ((p = morecore(nunits)) == NULL) {
                return NULL;
			}
        }
    }
}
