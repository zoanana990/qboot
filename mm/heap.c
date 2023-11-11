#include <qubitas/type.h>
#include <qubitas/utils.h>
#include <qubitas/printk.h>
#include <mm/heap.h>
#include <ds/list.h>

struct mm_block {
    /* the memory block linked list */
    struct list_node node;

    /* the size of free blocks */
    size_t block_size;
};

/**
 * The heap implementation is reference from FreeRTOS heap4
 * **/
static u8 heap[CONFIG_HEAP_SIZE];

/* Create a couple of linked list to mask the start and end of the list */
static struct mm_block heap_start;
static struct mm_block *ptr_heap_end = NULL;

/* keep track of the number of calls to allocate and free memory as well as the number
 * of free bytes remaining
 * */
static size_t free_bytes_remain = 0;
static size_t min_ever_free_bytes = 0;
//static size_t number_of_success_alloc = 0;
//static size_t number_of_success_free = 0;

/**************************************
 * PRIVATE FUNCTION
 **************************************/
//static void _heap_insertBlockToFreeList(struct mm_block *ptr_insert);

/**************************************
 * EXPORT API
 **************************************/
void mm_init(void) {
    u32 heap_start_addr, heap_end_addr;
    u32 total_heap_size = CONFIG_HEAP_SIZE;

    /* get the heap start address */
    heap_start_addr = (u32) heap;

    /* check the heap start on correct aligned boundary */
    if (unlikely((heap_start_addr & BYTE_ALIGNMENT_MASK) != 0)) {

        /* if the heap size is not alignment, add one byte alignment,
         * and the total size will minus back
         * */
        heap_start_addr += BYTE_ALIGNMENT - 1;
        heap_start_addr &= ~((u32) BYTE_ALIGNMENT_MASK);
        total_heap_size -= (size_t)(heap_start_addr - (u32) heap);
    }

    /* initialize the start block linked list head,
     * set the node next is start pointer, and the size is zero
     * */
    heap_start.node.next = (void *)heap_start_addr;
    heap_start.block_size = 0;

    /* initialize heap_end, insert the `struct heap_region`
     * to the tail of memory */
    heap_end_addr = heap_start_addr + (u32)total_heap_size;
    heap_end_addr -= (u32) sizeof(struct heap_region);
    heap_end_addr &= ~((u32)BYTE_ALIGNMENT_MASK);

    ptr_heap_end = (struct mm_block *) heap_end_addr;
    ptr_heap_end->block_size = 0;
    ptr_heap_end->node.next = NULL;

    /* To start with there is a single free block that is sized to take up
     * up the entire heap space, minus the space taken by ptr_end
     * */

    /* only one block exists, it cover entire size */
    min_ever_free_bytes = (size_t) (heap_end_addr - (u32) heap_start_addr);
    free_bytes_remain = (size_t) (heap_end_addr - (u32) heap_start_addr);

    pr_info("min_ever_free_bytes = %#x\n", min_ever_free_bytes);
    pr_info("free_bytes_remain = %#x\n", free_bytes_remain);
}

void *malloc(size_t size) {
    struct mm_block *ptr_block;
    struct mm_block *ptr_prev_block;

    return NULL;
}

void free(void *) {

}