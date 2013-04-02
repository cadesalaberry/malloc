malloc
======

Implementation of a malloc library with memory optimisation.

Two optimisation profiles are available:
- quick: the malloc will get the first available memory spot.
- best : will get the memory spot the closest in size.

Notice that in order to have the lowest memory footprint,
the metadata memory block only contains a pointer to the next free block,
and the size of the data following it. (no previous block)

# How To Run

Just add the following line in your code to be able to use the library.

	#include "mymalloc.h"

if you want to test the code, you can just run the tester:

	./test.sh
