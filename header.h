#define NALLOC 1024

// To align to long boundary
typedef long Align;

union header {
    // Block header
    struct {
		
        union header * ptr;			// Next block if on free list
        size_t size;				// Size of the block
	} s;
	
    // forces alignment of blocks
	Align x;
};
typedef union header Header;

static Header * morecore(unsigned nu);
static Header base;
static Header * freep = NULL;

