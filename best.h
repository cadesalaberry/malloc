#define INT_MAX 32767
/**
 * looks for the smallest free block sufficiently large
 * to store nbytes and returns its adress.
 */
void * malloc_best(size_t nbytes) {

    Header * p, * prevp;
    Header * morecore(unsigned);
    unsigned nunits, min_size = INT_MAX;
    Header * minp = NULL, * minprevp = NULL;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    
    // Creates the freelist if it does not exist
    if ((prevp = freep) == NULL) {
    
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }


	// Looks for the smallest block that is large enough to hold nbytes of data.
    for (p = prevp->s.ptr; /*No end condition*/; prevp = p, p = p->s.ptr) {
        
        // Checks if it is big enough 
        if (p->s.size >= nunits) {
			
			// If it is exactly the same size, just swap
            if (p->s.size == nunits) {
				
                prevp->s.ptr    = p->s.ptr;
                freep           = prevp;
                
                return (void *) (p + 1);
			
			} else {
				
                // Records the smallest block for now
                if (minp == NULL || p->s.size < min_size) {

                    minp        = p;
                    minprevp    = prevp;
                    min_size    = minp->s.size;
                }
            }
        }
		
		// Wrapped around free list
        if (p == freep) {
            
            if (minp != NULL) {
				
                // Fills in the tail end
                minp->s.size    -= nunits;
                minp            += minp->s.size;
                minp->s.size     = nunits;
                
                freep            = minprevp;
                
                return (void *)(minp + 1);
            }
            
			// Returns NULL if no space is left
            if ((p = morecore(nunits)) == NULL) {
                return NULL;
            }
        }
    }
}
