#include <qubitas/type.h>
#include <qubitas/utils.h>
#include <mm/heap.h>
#include <ds/list.h>

#define MAX_HEAPS 4096

struct mm_block {
    struct list_head head;
    u32 size;
};

static u8 heaps[MAX_HEAPS];
static u8 *mm_break = heaps;

static struct mm_block mm_base;
static struct list_head *ptr_free = NULL; /* start of free list */

void mm_init(void) {
    INIT_LIST_HEAD(&mm_base.head);
    mm_base.size = 0;
    ptr_free = &mm_base.head;
}

static void *sbrk(u32 nbytes)
{
    if (mm_break + nbytes >= heaps
        && mm_break + nbytes < heaps + MAX_HEAPS) {
        u8 *prev_break = mm_break;
        mm_break += nbytes;
        return (void *) prev_break;
    }
    return (void *) -1;
}

void *kalloc(size_t nbytes) {
    struct list_head *ptr_curr, *ptr_prev;
    u32 nunits;
    void *cp = NULL;

    /* Calculate the lowest size, including sizeof(struct mm_block) + real_malloc_size + padding
     * In this program, there are 16 bytes per unit.
     * */
    nunits = (nbytes + sizeof(struct mm_block) - 1) / sizeof(struct mm_block) + 1;

    /* let the ptr_prev = ptr_free */
    ptr_prev = ptr_free;

    for (ptr_curr = ptr_prev->next; ; ptr_prev = ptr_curr, ptr_curr = ptr_curr->next) {
        struct mm_block *ptr_curr_block = list_entry(ptr_curr, struct mm_block, head);
        if (ptr_curr_block->size >= nunits) {
            if (ptr_curr_block->size == nunits) {
                ptr_prev->next = ptr_curr->next;
            } else {
                ptr_curr_block->size -= nunits;
                ptr_curr_block += ptr_curr_block->size;
                ptr_curr_block->size = nunits;
            }
            ptr_free = ptr_prev;
            return (void *)(ptr_curr_block + 1);
        }

        if (ptr_curr == ptr_free) {
            /* Break the boundary of heap */
            cp = sbrk(nunits * sizeof(struct mm_block));
            if (cp == (void *) -1) {
                return NULL;
            } else {
                /* if breaking successfully,
                 * then we update the unit to the structure */
                ptr_curr_block = (struct mm_block *) cp;
                ptr_curr_block->size = nunits;
                kfree((void *) (ptr_curr_block + 1));
                ptr_curr = ptr_free;
            }
        }
    }
}

void kfree(void *p) {
    struct list_head *ptr_curr;
    struct mm_block *ptr_curr_block;
    struct mm_block *ptr_this_block = (struct mm_block *) p - 1;;

    for (ptr_curr = ptr_free; !(&ptr_this_block->head > ptr_curr && &ptr_this_block->head < ptr_curr->next);
         ptr_curr = ptr_curr->next) {
        if (ptr_curr >= ptr_curr->next && (&ptr_this_block->head > ptr_curr || &ptr_this_block->head < ptr_curr->next))
            break;
    }

    ptr_curr_block = list_entry(ptr_curr, struct mm_block, head);

    /* Merge two blocks */
    if (ptr_this_block + ptr_this_block->size == ptr_curr_block) {
        ptr_this_block->size += ptr_curr_block->size;
        ptr_this_block->head.next = ptr_curr_block->head.next->next;
    } else {
        ptr_this_block->head.next = ptr_curr_block->head.next;
    }

    /* Merge previous block */
    if (ptr_curr_block + ptr_curr_block->size == ptr_this_block) {
        ptr_curr_block->size += ptr_this_block->size;
        ptr_curr_block->head.next = ptr_this_block->head.next;
    } else {
        ptr_curr_block->head.next = &ptr_this_block->head;
    }

    /* update the ptr_curr->ptr in ptr_free */
    ptr_free = ptr_curr;
}