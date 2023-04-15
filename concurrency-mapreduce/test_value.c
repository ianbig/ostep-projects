#include <stdio.h>

#include "hashmap.h"

int main() {
    int values[] = {1, 2, 3, 4, 5};
    value_t * values_arr = value_create();
    for (int i = 0; i < 5; i++) {
        value_append(values_arr, values[i]);
    }

    value_print(values_arr);
    value_destroy(values_arr);

    return 0;
}