#include <stdio.h>

#define STRING_IMPLEMENTATION
#define VECTOR_IMPLEMENTATION
#include "base.h"

VECTOR(uint8_t, byte_vector)

int main(void) {
    printf("Hello, World!\n");

    string_t str = stralloc(12);
    straddcs(&str, "Hello, World! But Another\n");
    printf("%s", strascstr(str));
    printf("%ld\n", str.length);
    printf("%ld\n", str.capacity);

    string_t another_str = S("asdasd");
    straddcs(&another_str, "zz\n");

    printf("%s", strascstr(another_str));
    printf("%ld\n", another_str.length);
    printf("%ld\n", another_str.capacity);
    
    byte_vector_t vec = {0};
    uint8_t fav_letter = 'A';

    for (uint8_t i = 'a'; i <= 'z'; i++) {
        vecpush(vec, i);
    }

    vecpop(vec);
    vecinsert(vec, fav_letter, 5);

    foreach(vec, c) {
        putchar(*c);
    }
    putchar(0xa);

    printf("Array len: %ld\n", vec.length);
    printf("Array capacity: %ld\n", vec.capacity);

    strfree(str);
    strfree(another_str);

    return 0;
}