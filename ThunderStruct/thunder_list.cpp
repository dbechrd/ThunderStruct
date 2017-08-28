#include <stdlib.h>
#include <stdio.h>

#define LIST_MIN_CAPACITY 16
#define LIST_MAX_GROWTH 4096

ThunderList *thunder_list_init(uint32_t capacity)
{
    if (capacity < LIST_MIN_CAPACITY)
    {
        capacity = LIST_MIN_CAPACITY;
    }
    else
    {
        // Find next power of 2
        uint32_t power = LIST_MIN_CAPACITY;
        while (power < capacity)
        {
            power <<= 1;
        }
        capacity = power;
    }

    ThunderList *list = (ThunderList *)calloc(1, sizeof(ThunderList));
    list->size = 0;
    list->capacity = capacity;
    list->elements = (void **)calloc(list->capacity, sizeof(void *));
    return list;
}

ThunderList *thunder_list_init()
{
    return thunder_list_init(LIST_MIN_CAPACITY);
}

void thunder_list_grow(ThunderList *list)
{
    printf("ThunderList: Growing from %d elements to ", list->capacity);
    // Double size until MAX_GROWTH, then allocate in fixed-size chunks
    if (list->capacity < LIST_MAX_GROWTH)
    {
        list->capacity = list->capacity * 2;
    }
    else
    {
        list->capacity = list->capacity + LIST_MAX_GROWTH;
    }
    printf("%d elements.\n", list->capacity);

    void **old_list = list->elements;
    list->elements = (void **)calloc(list->capacity, sizeof(void *));

    for (uint32_t i = 0; i < list->size; ++i)
    {
        list->elements[i] = old_list[i];
    }

    free(old_list);
}

void thunder_list_add(ThunderList *list, void *value)
{
    if (list->size == list->capacity)
    {
        thunder_list_grow(list);
    }

    list->elements[list->size++] = value;
}

void *thunder_list_read(ThunderList *list, uint32_t index)
{
    if (index >= 0 && index < list->size)
    {
        return list->elements[index];
    }
    else
    {
        return NULL;
    }
}

void thunder_list_remove_at(ThunderList *list, uint32_t index)
{
    list->elements[index] = list->elements[--list->size];
}

void thunder_list_remove(ThunderList *list, void *value)
{
    for (uint32_t index = 0; index < list->size; ++index)
    {
        if (list->elements[index] == value)
        {
            thunder_list_remove_at(list, index);
            break;
        }
    }
}

void thunder_list_free(ThunderList *list)
{
    free(list->elements);
    free(list);
}