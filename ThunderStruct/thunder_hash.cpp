#include <stdlib.h>
#include <stdio.h>

#define THUNDER_ASSERT(exp) if(!(exp)) {*(int*)0=0;}

#define HASH_MIN_CAPACITY 16
#define HASH_MAX_GROWTH 4096
#define HASH_MAX_CHAIN 8

ThunderHash *thunder_hash_init(int capacity)
{
    if (capacity < HASH_MIN_CAPACITY)
    {
        capacity = HASH_MIN_CAPACITY;
    }
    else
    {
        // Find next power of 2
        uint32_t power = HASH_MIN_CAPACITY;
        while (power < capacity)
        {
            power <<= 1;
        }
        capacity = power;
    }

    ThunderHash *hash = (ThunderHash *)calloc(1, sizeof(ThunderHash));
    hash->capacity = capacity;
    hash->slots = (ThunderHashItem *)calloc(hash->capacity,
                                            sizeof(ThunderHashItem));
    return hash;
}

// https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
inline uint32_t murmur3_fmix32(uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

int thunder_hash_hashcode(ThunderHash *hash, uint32_t key)
{
    return murmur3_fmix32(key) % hash->capacity;
    //return key % hash->capacity;
}

void thunder_hash_insert(ThunderHash *hash, uint32_t key, void *value,
                         bool resizing);
void thunder_hash_grow(ThunderHash *hash)
{
    printf("ThunderHash: Growing from %d elements to ", hash->capacity);

    uint32_t old_capacity = hash->capacity;
    ThunderHashItem *old_slots = hash->slots;

    // Double size until MAX_GROWTH, then allocate in fixed-size chunks
    if (hash->capacity < HASH_MAX_GROWTH)
    {
        hash->capacity = hash->capacity * 2;
    }
    else
    {
        hash->capacity = hash->capacity + HASH_MAX_GROWTH;
    }
    printf("%d elements.\n", hash->capacity);

    hash->slots = (ThunderHashItem *)calloc(hash->capacity,
                                            sizeof(ThunderHashItem));

    ThunderHashItem *item;
    ThunderHashItem *next;
    for (uint32_t i = 0; i < old_capacity; ++i)
    {
        item = &old_slots[i];

        if (item->value != NULL)
        {
            thunder_hash_insert(hash, item->key, item->value, true);
            item = item->next;

            while (item != NULL)
            {
                next = item->next;
                thunder_hash_insert(hash, item->key, item->value, true);
                free(item);
                item = next;
            }
        }
    }

    free(old_slots);
}

void thunder_hash_insert(ThunderHash *hash, uint32_t key, void *value)
{
    thunder_hash_insert(hash, key, value, false);
}

void thunder_hash_insert(ThunderHash *hash, uint32_t key, void *value,
                         bool resizing)
{
    int hashcode = thunder_hash_hashcode(hash, key);
    ThunderHashItem *item = &hash->slots[hashcode];

    int chain_size = 1;
    while (item->value != NULL && item->next != NULL)
    {
        item = item->next;
        chain_size++;
    }

    // Resize when chains grow too long
    if (!resizing && chain_size == HASH_MAX_CHAIN)
    {
        thunder_hash_grow(hash);

        hashcode = thunder_hash_hashcode(hash, key);
        item = &hash->slots[hashcode];
        while (item->value != NULL && item->next != NULL)
        {
            item = item->next;
            chain_size++;
        }
    }

    if (item->value == NULL)
    {
        hash->slots[hashcode].key = key;
        hash->slots[hashcode].value = value;
    }
    else
    {
        ThunderHashItem *newitem =
            (ThunderHashItem *)calloc(1, sizeof(ThunderHashItem));
        newitem->key = key;
        newitem->value = value;

        printf("Alloc key %d at %p\n", key, newitem);

        item->next = newitem;
    }
}

void *thunder_hash_retrieve(ThunderHash *hash, uint32_t key)
{
    int hashcode = thunder_hash_hashcode(hash, key);

    ThunderHashItem *item = &hash->slots[hashcode];
    while (item != NULL && item->key != key)
    {
        item = item->next;
    }

    if (item == NULL)
    {
        return NULL;
    }

    return item->value;
}

void thunder_hash_delete(ThunderHash *hash, uint32_t key)
{
    int hashcode = thunder_hash_hashcode(hash, key);

    ThunderHashItem *item = &hash->slots[hashcode];
    if (item->value == NULL)
    {
        return;
    }

    ThunderHashItem *prev_item = NULL;
    while (item != NULL && item->key != key)
    {
        prev_item = item;
        item = item->next;
    }

    // Item found
    if (item != NULL)
    {
        if (prev_item != NULL)
        {
            // If in chain, do linked list fix-up, then delete item
            prev_item->next = item->next;
            free(item);
        }
        else if (item->next != NULL)
        {
            // If in slot, copy first item in chain to slot, then delete it
            ThunderHashItem *next = item->next;
            hash->slots[hashcode] = *next;
            THUNDER_ASSERT(next != &hash->slots[hashcode]);
            free(next);
        }
        else
        {
            // If in slot and chain is empty, just clear the slot
            hash->slots[hashcode] = { 0 };
        }
    }
}

void thunder_hash_free(ThunderHash *hash)
{
    free(hash->slots);
    free(hash);
}