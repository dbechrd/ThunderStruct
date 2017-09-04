#include <stdio.h>
#include "stretchy_buffer.h"

typedef struct blah {
    int x, y;
} Blah;

#include "thunder.cpp"
#include "thunder_test.cpp"

int main(int argc, char *argv[])
{
    init_test();
    //thunder_hash_test();
    thunder_list_test();

    getchar();
    return 0;
}