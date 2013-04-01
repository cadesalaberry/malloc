#define NALLOC 1024

union header {					    // Block header
    struct {
		
        union header * nxt;			// Next block if on free list
        size_t size;				// Size of the block
	} h;
};
typedef union header Header;

static Header * morecore(unsigned int nu);
static Header 	base;
static Header * freep = NULL;

