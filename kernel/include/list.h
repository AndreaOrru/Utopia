#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct Link
{
    struct Link* next;
    struct Link* prev;
} Link;
typedef Link List;

#define LINK(name)  { &name, &name }

#define LIST(name) \
    List name = LINK(name);

#define list_item(link, type, member) \
    ((type*) ((void*)(link) - (void*)&(((type*)NULL)->member)))

#define list_foreach(list, it) \
    for (Link* it = (list)->next; it != (list); it = it->next)

static inline void list_init(List* list)
{
    list->next = list->prev = list;
}

static inline Link* list_first(const List* list)
{
    return list->next;
}

static inline Link* list_last(const List* list)
{
    return list->prev;
}

static inline bool list_empty(const List* list)
{
    return list->next == list;
}

static inline void _list_insert(Link* prev, Link* next, Link* new)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_insert(Link* entry, Link* new)
{
    _list_insert(entry, entry->next, new);
}

static inline void list_insert_before(Link* entry, Link* new)
{
    _list_insert(entry->prev, entry, new);
}

static inline void list_append(List* list, Link* new)
{
    list_insert_before(list, new);
}

static inline void list_prepend(List* list, Link* new)
{
    list_insert(list, new);
}

static inline void list_remove(Link* entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;

    entry->next = entry->prev = NULL;
}

static inline Link* list_pop(List* list)
{
    Link* first = list_first(list);
    list_remove(first);

    return first;
}
