typedef struct thunder_list
{
    uint32_t size;      // size of element
    uint32_t count;     // count of elements in list
    uint32_t capacity;  // capacity of list (in elements)
    void *data;
} ThunderList;
