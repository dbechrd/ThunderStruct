#include "thunder.cpp"
#include "thunder_test.cpp"
#include <stdio.h>

int main(int argc, char *argv[])
{
    init_test();
    thunder_hash_test();
    //thunder_list_test();

    getchar();
    return 0;
}