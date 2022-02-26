#pragma once
#include <geometry/geometry.h>
#include <stdlib.h>
// Doubly linked list struct for Shape *
struct ShapeList_Node
{
    const Shape *object;
    struct ShapeList_Node *next;
    struct ShapeList_Node *prev;
};

struct ShapeList
{
    struct ShapeList_Node *head;
    struct ShapeList_Node *tail;
    int length;
};
static struct ShapeList init_ShapeList(void);
static void destroy_ShapeList(struct ShapeList *list);
static struct ShapeList_Node *create_node(const Shape *obj);
static struct ShapeList_Node *find_obj(struct ShapeList *list,
                                       const Shape *obj);
static void add_obj(struct ShapeList *list, const Shape *obj);
static void delete_obj_from_list(struct ShapeList *list,
                                 struct ShapeList_Node *np);

static struct ShapeList_Node *create_node(const Shape *obj)
{
    struct ShapeList_Node *np = malloc(sizeof(*np));
    np->object = obj;
    np->next = NULL;
    np->prev = NULL;
    return np;
}

static struct ShapeList init_ShapeList(void)
{
    struct ShapeList list;
    list.head = NULL;
    list.tail = NULL;
    list.length = 0;
    return list;
}

static void destroy_ShapeList(struct ShapeList *list)
{
    struct ShapeList_Node *np, *tmp;
    np = list->head;
    // Traverse the list and free heap memory occupied by nodes
    while (np)
    {
        tmp = np->next;
        free(np);
        np = tmp;
    }
}

static void add_obj(struct ShapeList *list, const Shape *obj)
{
    struct ShapeList_Node *np;
    if (!list->head)
    {
        np = create_node(obj);
        list->head = np; // Only one node for both head
        list->tail = np; // and tail
        list->length++;  // Increment length
        return;
    }
    struct ShapeList_Node *new = create_node(obj);
    list->tail->next = new; // add to tail
    new->prev = list->tail; // link back
    list->tail = new;       // Tell the list the new tail
    list->length++;         // Increment length
}

static struct ShapeList_Node *find_obj(struct ShapeList *list, const Shape *obj)
{
    if (list->length == 0) // Skip when list is empty
        return NULL;
    if (list->head->object == obj) // Check if head contains
        return list->head;
    if (list->tail->object == obj) // Check if tail contains
        return list->tail;
    struct ShapeList_Node *np;
    for (np = list->head; np; np = np->next) // Traverse the list
    {
        if (np->object == obj) // Found and return
            return np;
    }
    return NULL; // Not found
}

static void delete_obj_from_list(struct ShapeList *list,
                                 struct ShapeList_Node *np)
{
    if (np->prev) // If np->prev exists
    {             // Check if next exists
        if (np->next)
        {
            np->prev->next =
                np->next; // Connect the two nodes around the del node
            np->next->prev = np->prev;
        }
        else // End of list
        {
            np->prev->next = NULL; // Point to NULL
            list->tail = np->prev; // Set tail to the np->prev
        }
    }
    else // np->prev is NULL, means np is at head
    {
        if (np->next) // np->next exists
        {
            np->next->prev = NULL; // Make np->next new head by setting
                                   // (np->next)->prev to NULL
            list->head = np->next; // New head
        }
        else
        {
            list->head = NULL; // Only one node
            list->tail = NULL; // Back to empty state
        }
    }
    list->length--; // Decrement length
    free(np);       // free heap memory allocated in create_node()
    return;
}
