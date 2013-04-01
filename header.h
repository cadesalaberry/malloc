#define NALLOC 1024

// To align to long boundary
typedef long Align;

union header {						// Block header
    struct {
		
        union header * ptr;			// Next block if on free list
        size_t size;				// Size of the block
	} s;
	
	Align x;						// Forces alignment of blocks
};
typedef union header Header;

void * malloc(size_t size);

void free(void * ptr);

void * realloc(void * ptr, size_t size);

static Header * morecore(unsigned nu);
