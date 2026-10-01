#include "bass/base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_t cstrasstr(const char *cstr) {
    size_t len = strlen(cstr);
    
    string_t str = stralloc(len + 1); // adding the nullptr
    straddcs(&str, cstr);

    return str;
}

string_t stralloc(uint64_t capacity) {
    char *ptr = malloc(capacity);
    
    if (ptr == NULL) {
        perror("stralloc, ptr allocation failed");
        exit(1);
    }

    string_t str = {
        .ptr = ptr,
        .length = 0,
        .capacity = capacity
    };

    return str;
}

void straddc(string_t *str, char c) { 
    if (str->length + 2 >= str->capacity) {
        str->ptr = realloc(str->ptr, str->capacity * 2);

        str->capacity *= 2;
    }

    *(str->ptr + str->length) = c;
    *(str->ptr + str->length + 1) = 0;
    str->length++;
}

void straddcs(string_t *str, const char *cstr) {
    while (*cstr) {
        straddc(str, *cstr);
        cstr++;
    }
}

void stradds(string_t *str, string_t other) {
    straddcs(str, other.ptr);
}

void strfree(string_t str) {
    free(str.ptr);
}

const char *strascstr(string_t str) {
    return str.ptr;
}

// taken from https://github.com/TomasBorquez/base.h/blob/master/base.h. Can view licence from the repo
#define DEFINE_VECTOR(type, name)          \
    typedef struct name {           \
        size_t capacity;            \
        size_t length;              \
        type *data;                 \
    } name##_t;                     \


void __base_vec_push(void **data, size_t *length, size_t *capacity, size_t element_size, void *value) {
    // WARNING: Vector must always be initialized to zero `Vector vector = {0}`
    assert(*length <= *capacity, "__base_vec_push: Possible memory corruption or vector not initialized, `Vector vector = {0}`");
    assert(!(*length > 0 && *data == NULL), "__base_vec_push: Possible memory corruption, data should be NULL only if length == 0");

    if (*length >= *capacity) {
        if (*capacity == 0) *capacity = 128;
        else *capacity *= 2;

        *data = realloc(*data, *capacity * element_size);
    }

    void *address = (char *)(*data) + (*length * element_size);
    memcpy(address, value, element_size);

    (*length)++;
}

void *__base_vec_pop(void *data, size_t *length, size_t element_size) {
    assert(*length > 0, "__base_vec_pop: Cannot pop from empty vector");
    (*length)--;
    return (char *)data + (*length * element_size);
}

void __base_vec_insert(void **data, size_t *length, size_t *capacity, size_t element_size, void *value, size_t index) {
    assert(index <= *length, "__base_vec_insert: Index out of bounds for insertion");

    if (*length >= *capacity) {
        if (*capacity == 0) *capacity = 2;
        else *capacity *= 2;
        *data = realloc(*data, *capacity * element_size);
    }

    if (index < *length) {
        memmove((char *)(*data) + ((index + 1) * element_size), (char *)(*data) + (index * element_size), (*length - index) * element_size);
    }

    memcpy((char *)(*data) + (index * element_size), value, element_size);
    (*length)++;
}

void *__base_vec_at(void **data, size_t *length, size_t index, size_t elementSize) {
    assert(index < *length, "__base_vec_at: Index out of bounds");
    void *address = (char *)(*data) + (index * elementSize);
    return address;
}

void __base_vec_free(void **data, size_t *length, size_t *capacity) {
    free(*data);
    *data = NULL;
    *length = 0;
    *capacity = 0;
}