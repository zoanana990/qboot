#include <qubitas/type.h>
#include <qubitas/utils.h>
#include <qubitas/printk.h>
#include <mm/heap.h>
#include <ds/list.h>
#include <kernel/task.h>

struct mm_block {
    /* the memory block linked list */
    struct mm_block *ptr_next_free_block;

    /* the size of free blocks */
    size_t block_size;
};

/**
 * The heap implementation is reference from FreeRTOS heap4
 *
 * TODO: Need to implement in linux style linked list
 * **/
static const size_t heap_struct_size = (sizeof(struct mm_block) + ((size_t) BYTE_ALIGNMENT_MASK)) &
                                        ~((size_t) BYTE_ALIGNMENT_MASK);

static u8 heap[CONFIG_HEAP_SIZE];

/* Create a couple of linked list to mask the start and end of the list */
static struct mm_block heap_start;
static struct mm_block *ptr_heap_end = NULL;

/* keep track of the number of calls to allocate and free memory as well as the number
 * of free bytes remaining
 * */
static size_t free_bytes_remain = 0;
static size_t min_ever_free_bytes = 0;
static size_t number_of_success_alloc = 0;
//static size_t number_of_success_free = 0;

/**************************************
 * MACROS
 **************************************/
#define HEAP_MINIMA_BLOCK                   ((size_t)(heap_struct_size << 1))

#define CHECK_HEAP_OVERFLOW(__ALLOC_SIZE__, __BOUNDARY__) \
    ((__ALLOC_SIZE__) > (CONFIG_HEAP_SIZE - (__BOUNDARY__)))

#define CHECK_HEAP_UNDERFLOW(__ALLOC_SIZE__, __BOUNDARY__) \
    ((__ALLOC_SIZE__) < (__BOUNDARY__))

#define CHECK_ALLOC_SIZE_ALIGN(__ALLOC_SIZE__, __ALIGN_MASK__) \
    ((__ALLOC_SIZE__) & (__ALIGN_MASK__))

#define CHECK_HEAP_SIZE_IS_VALID(__SIZE__) \
    (((__SIZE__) & (WORD_ALIGNMENT_MASK)) == 0)

#define VERIFY_HEAP_BLOCK_POINTER(__PTR_BLOCK__)        \
    ASSERT( ((u8 *) (__PTR_BLOCK__) >= &(heap[0])) &&   \
            ((u8 *) (__PTR_BLOCK__) <= &(heap[CONFIG_HEAP_SIZE - 1])))

/**************************************
 * PRIVATE FUNCTION
 **************************************/
static void _heap_insertBlockToFreeList(struct mm_block *ptr_insert)
{

}

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
    heap_start.ptr_next_free_block = (void *)heap_start_addr;
    heap_start.block_size = 0;

    /* initialize heap_end, insert the `struct heap_region`
     * to the tail of memory */
    heap_end_addr = heap_start_addr + (u32)total_heap_size;
    heap_end_addr -= (u32) heap_struct_size;
    heap_end_addr &= ~((u32)BYTE_ALIGNMENT_MASK);

    ptr_heap_end = (struct mm_block *) heap_end_addr;
    ptr_heap_end->block_size = 0;
    ptr_heap_end->ptr_next_free_block = NULL;

    /* To start with there is a single free block that is sized to take up
     * up the entire heap space, minus the space taken by ptr_end
     * */

    /* only one block exists, it cover entire size */
    min_ever_free_bytes = (size_t) (heap_end_addr - (u32) heap_start_addr);
    free_bytes_remain = (size_t) (heap_end_addr - (u32) heap_start_addr);

    pr_info("heap_start_address = %#x\n", heap_start_addr);
    pr_info("min_ever_free_bytes = %#x\n", min_ever_free_bytes);
    pr_info("free_bytes_remain = %#x\n", free_bytes_remain);
}

void *malloc(size_t size) {

    struct mm_block *ptr_block;
    struct mm_block *ptr_prev_block;
    struct mm_block *ptr_new_block;

    void * ptr_result = NULL;
    u32 additional_size;

    /* 1. Check the size will not overflow */
    if(unlikely(CHECK_HEAP_OVERFLOW(size, CONFIG_HEAP_SIZE - sizeof(struct heap_region)))) {
        size += heap_struct_size;

        /* 1.1 Check the address is alignment */
        if(unlikely(CHECK_ALLOC_SIZE_ALIGN(size, BYTE_ALIGNMENT_MASK))) {
            additional_size = BYTE_ALIGNMENT - (size & BYTE_ALIGNMENT_MASK);

            if(unlikely(CHECK_ALLOC_SIZE_ALIGN(size, additional_size) == 0)) {
                size += additional_size;
            } else {
                return NULL;
            }
        }
    } else {
        return NULL;
    }

    TASK_SUSPEND;
    /* Check the block size, we are trying to allocate is not so large that the
     * top bit is set.
     * */
    if(CHECK_HEAP_SIZE_IS_VALID(size) != 0) {
        if((size > 0) && (size <= free_bytes_remain)) {

            ptr_prev_block = &heap_start;
            ptr_block->ptr_next_free_block = heap_start.ptr_next_free_block;

            /* traverse the list from the start block until one of adequate size is found
             * the first find algorithm
             * */
            while((ptr_block->block_size < size) && (ptr_block->ptr_next_free_block != NULL)) {

                ptr_prev_block = ptr_block;
                ptr_block = ptr_block->ptr_next_free_block;
                VERIFY_HEAP_BLOCK_POINTER(ptr_block);
            }

            if(ptr_block != ptr_heap_end) {

                /* return the memory space pointed to jumping over the struct mm_block at its start */
                ptr_result = (void *) ((u8 *) (ptr_prev_block->ptr_next_free_block) + heap_struct_size);
                VERIFY_HEAP_BLOCK_POINTER(ptr_block);

                /* This block is returned for use so must be taken out of the list of free blocks */
                ptr_prev_block->ptr_next_free_block = ptr_block->ptr_next_free_block;

                /* Check this block is larger than the required size */
                ASSERT(CHECK_HEAP_UNDERFLOW(ptr_block->block_size, size) == 0);

                if(ptr_block->block_size - size > HEAP_MINIMA_BLOCK) {
                    /* This block is to be split into two.
                     * Create a new block following the number of bytes requested
                     *
                     * The void case is used to prevent byte alignment warnings from the compiler
                     * */
                    ptr_new_block = (void *) (((u8 *) ptr_block) + size);

                    /* Calculate the sizes of two blocks split form the single block */
                    ptr_new_block->block_size = ptr_new_block->block_size - size;
                    ptr_block->block_size = size;

                    /* insert the linked list node */
                    ptr_new_block->ptr_next_free_block = ptr_prev_block->ptr_next_free_block;
                    ptr_prev_block->ptr_next_free_block = ptr_new_block;
                }

                free_bytes_remain -= ptr_block->block_size;

                ptr_block->ptr_next_free_block = NULL;
                number_of_success_alloc++;
            }
        }
    }
    TASK_RESUME;

    return ptr_result;
}

void free(void *ptr) {

    TASK_SUSPEND;
    _heap_insertBlockToFreeList(NULL);
    TASK_RESUME;
}