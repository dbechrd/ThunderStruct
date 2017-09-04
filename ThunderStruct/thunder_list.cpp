#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LIST_MIN_CAPACITY 16
#define LIST_MAX_GROWTH 4096

void thunder_list_grow(ThunderList *list)
{
    printf("ThunderList: Growing from %d elements to ", list->capacity);
    // Double size until MAX_GROWTH, then allocate in fixed-size chunks

    if (list->capacity == 0)
    {
        list->capacity = LIST_MIN_CAPACITY;
    }
    if (list->capacity < LIST_MAX_GROWTH)
    {
        list->capacity = list->capacity * 2;
    }
    else
    {
        list->capacity = list->capacity + LIST_MAX_GROWTH;
    }
    printf("%d elements.\n", list->capacity);

    list->data = realloc(list->data, list->capacity * list->size);
}

void thunder_list_init(ThunderList *list, uint32_t size)
{
    list->size = size;
    thunder_list_grow(list);
}

void thunder_list_add(ThunderList *list, void *value)
{
    if (list->count == list->capacity)
    {
        thunder_list_grow(list);
    }

    void *dst = (char *)list->data + list->size * list->count++;
    memcpy(dst, value, list->size);
}

void *thunder_list_read(ThunderList *list, uint32_t index)
{
    if (index >= 0 && index < list->count)
    {
        return (char *)list->data + list->size * index;
    }
    else
    {
        return NULL;
    }
}

void thunder_list_remove_at(ThunderList *list, uint32_t index)
{
    void *at = (char *)list->data + list->size * index;
    void *last = (char *)list->data + list->size * --list->count;
    memcpy(at, last, list->size);
}

/*
// TODO: Requires pointer to comparator, e.g. equals(void *a, void *b)
void thunder_list_remove(ThunderList *list, void *value)
{
    for (uint32_t index = 0; index < list->count; ++index)
    {
        void *val = list->data + list->size * index;
        if (val == value)
        {
            thunder_list_remove_at(list, index);
            break;
        }
    }
}
*/