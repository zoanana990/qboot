#include <qubitas/type.h>
#include <mm/heap.h>
#include <ds/list.h>

/**
 * The heap implementation is reference from CSAPP version 3 chapter 9's method
 * **/

static u8 heap[CONFIG_HEAP_SIZE];

/* some global variable here */


/* heap init */

/* */