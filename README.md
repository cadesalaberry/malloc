malloc
======

Implementation of a malloc library with memory optimisation.

Two optimisation profiles are available:
- quick: the malloc will get the first available memory spot.
- best : will get the memory spot the closest in size.

# How To Run

Just add the following line in your code to be able to use the library.

	#include "mymalloc.h"

if you want to test the code, you can just run the tester:

	./test.sh
