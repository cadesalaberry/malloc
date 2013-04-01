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
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }


    // Gets a block large enough to hold nbytes of data.
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		
		// Checks if it is big enough
        if (p->s.size >= nunits) {
			
			// Checks if it is the same size
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            }
            else {
                // Allocates tail end
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
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
