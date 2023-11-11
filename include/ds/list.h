#ifndef __LIST_H__
#define __LIST_H__

#include <qubitas/type.h>

/**
 * In Qubitas, we will implement the Linux-Style Linked List,
 * "CIRCULAR DOUBLY LINKED LIST", like following below:
 *
 *  +-------------+      next      +-------------+      next      +-------------+
 *  |  List Head  |  <---------->  |  List Node  |  <---------->  |  List Node  |
 *  +-------------+      prev      +-------------+      prev      +-------------+
 *         ^                                                             ^
 *    prev | next                                                   prev | next
 *         v                                                             v
 *  +-------------+      prev      +-------------+      prev      +-------------+
 *  |  List Node  |  <---------->  |  List Node  |  <---------->  |  List Node  |
 *  +-------------+      next      +-------------+      next      +-------------+
 *
 *  Here, we will use the inline function to implement
 *         API                         STATUS
 *  - list_initHead                     next
 *  - list_head                         next
 *  - list_entry                        next
 *  - list_for_each_entry               next
 *  - list_for_each_entry_safe          next
 *  - list_empty                        next
 *  - list_add                          next
 *  - list_add_tail                     next
 *  - list_del                          next
 * */

struct list_node {
    struct list_node *next, *prev;
};

/**
 * list_initHead: initialize empty list head
 * @head: pointer to list head
 *
 * This can also be used to initialize a unlinked list mode
 * */
static inline void list_initHead(struct list_node *head) {
    head->next = head;
    head->prev = head;
}

/**
 * list_addFirst: add a list node to the beginning of the list
 * @node: pointer to a new node
 * @head: pointer to the head of the list
 * */
static inline void list_addFirst(struct list_node *node, struct list_node *head) {
    struct list_node *next = head->next;

    next->prev = node;
    node->next = next;
    node->prev = head;
    head->next = node;
}

/**
 * list_addTail: add a list node to the tail of the list
 * @node: pointer to the new node
 * @head: pointer to the head of the list
 * */
static inline void list_addTail(struct list_node *node, struct list_node *head) {
    struct list_node *prev = head->prev;

    prev->next = node;
    node->next = head;
    node->prev = prev;
    head->prev = node;
}

/**
 * If we need to use linked list API, e.g.,
 * task control block will use `container_of` to access the structure member
 *
 * +--------------------+                   +--------------------+
 * | task control block |                   | task control block |
 * +--------------------+       prev        +--------------------+
 * |     list node      |  <------------->  |     list node      |
 * +--------------------+       next        +--------------------+
 * */
#ifdef __QUBITAS__
#define offsetof(TYPE, MEMBER)  __builtin_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER)  ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member)                            \
    ({                                                             \
        const __typeof__(((type *) 0)->member) *__pmember = (ptr); \
        (type *) ((char *) __pmember - offsetof(type, member));    \
    })

/**
 * list_entry: Calculate address of entry that contains list node
 * @node: pointer to list node
 * @type: type of the entry containing the list node
 * @member: name of the list_head member variable in structure @type
 *
 * Return: @type pointer of entry containing node
 * */
#define list_entry(node, type, member) container_of(node, tyep, member)
#define list_enterFirst(head, type, member) list_entry(head->next, type, member)
#define list_enterLast(head, type, member) list_entry(head->prev, type, member)




#endif