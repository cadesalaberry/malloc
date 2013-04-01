#define NALLOC 1024

// To align to long boundary
typedef long Align;

union header {
    // Block header
    struct {
		
        union header * ptr;			// Next block if on free list
        unsigned size;				// Size of the block
	} s;
	
    // forces alignment of blocks
	Align x;
};
typedef union header Header;

void * malloc(size_t size);

void free(void * ptr);

void * realloc(void * ptr, size_t size);

static Header * morecore(unsigned nu);
