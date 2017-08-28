#include <stdio.h>

#define TEST_VALUE_COUNT 5000

int values[TEST_VALUE_COUNT];

void init_test()
{
    for (int i = 1; i <= TEST_VALUE_COUNT; ++i)
    {
        values[i] = i;
    }
}

void thunder_hash_test()
{
    ThunderHash *hash = thunder_hash_init(2);

    for (int i = 0; i < 30; ++i)
    {
        values[i] = i;
        thunder_hash_insert(hash, i, (void *)&values[i]);
    }
    for (int i = 20; i < 30; ++i)
    {
        thunder_hash_delete(hash, i);
    }

    printf("ThunderHash\n");
    for (int i = 0; i < 30; ++i)
    {
        int *value = (int *)thunder_hash_retrieve(hash, i);
        if (value != NULL)
        {
            printf("[%d] %d\n", i, *value);
        }
        else
        {
            printf("[%d] NULL\n", i);
        }
    }

    thunder_hash_free(hash);
}

void thunder_list_test()
{
    ThunderList *list = thunder_list_init(10);

    for (int i = 0; i < 30; ++i)
    {
        values[i] = i;
        thunder_list_add(list, (void *)&values[i]);
    }
    for (int i = 10; i < 20; ++i)
    {
        thunder_list_remove(list, &values[i]);
    }

    printf("ThunderList\n");
    for (int i = 0; i < 30; ++i)
    {
        int *value = (int *)thunder_list_read(list, i);
        if (value != NULL)
        {
            printf("[%d] %d\n", i, *value);
        }
        else
        {
            printf("[%d] NULL\n", i);
        }
    }

    thunder_list_free(list);
}