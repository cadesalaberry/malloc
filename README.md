malloc
======

Implementation of a malloc library with memory optimisation.

Two optimisation profiles are available:
* quick: the malloc will get the first available memory spot.
* best : will get the memory spot the closest in size.

Notice that in order to have the lowest memory footprint,
the metadata memory block only contains a pointer to the next free block,
and the size of the data following it. (no previous block)

The list indexing the available blocks in memory is a circular list.
The last block has **base** (the first free pointer) as its next.

This code has been developped and successfully executed on Fedora-17-x86_64.

# How To Run

Just add the following line in your code to be able to use the library.

	#include "mymalloc.h"

if you want to test the code, you can just run the tester:

	./test.sh

# Sample Output

	[******][lab7-17][~/Apps/malloc] ./test.sh 
	Hole finding test....
					 Passed

	Program break expansion test...
					 Passed
	Check for first fit algorithm.... 
					 Passed
	Check for best first algorithm.... 
					 Passed
	Print some information..
	Free blocks	: 8
	Free space 	: 8555
	Memory span	: 4588097
	[******][lab7-17][~/Apps/malloc] 

