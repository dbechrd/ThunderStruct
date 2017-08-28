#include <stdint.h>

typedef struct thunder_hash_item ThunderHashItem;

typedef struct thunder_hash_item
{
    uint32_t key;
    void *value;
    ThunderHashItem *next;
} ThunderHashItem;

typedef struct thunder_hash
{
    uint32_t capacity;
    ThunderHashItem *slots;
} ThunderHash;
