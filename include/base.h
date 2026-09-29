#ifndef BASE_H
#define BASE_H

#include <stdint.h>

#define log(level, fmt, ...) printf("[%s] `%s`@%d: " fmt "\n", level, __FILE__, __LINE__, __VA_ARGS__);

#define debug(fmt, ...) log("DEBUG", fmt, __VA_ARGS__)
#define info(fmt, ...) log("INFO", fmt, __VA_ARGS__)
#define warn(fmt, ...) log("WARNING", fmt, __VA_ARGS__)
#define error(fmt, ...) log("ERROR", fmt, __VA_ARGS__)
#define fatal(fmt, ...) log("FATAL", fmt, __VA_ARGS__)

#define assert(case, explaination) if (!(case)) { error("%s", explaination); exit(1); }

typedef struct string string_t;

string_t stralloc(uint64_t capacity);
void straddc(string_t *str, char c);
void straddcs(string_t *str, const char *cstr);
void stradds(string_t *str, string_t other);
void strfree(string_t str);
const char *strascstr(string_t str);

#ifdef STRING_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// taken from https://github.com/TomasBorquez/base.h/blob/master/base.h
#define TYPE_INIT(type) (type)
#define STRING_LENGTH(s) ((sizeof((s)) / sizeof((s)[0])) - sizeof((s)[0])) 
#define ENSURE_STRING_LITERAL(x) ("" x "")
// end taken

#define S(string) s(string); // inspired but not copied

typedef struct string {
    size_t capacity;
    size_t length;
    char *ptr;
} string_t;


string_t s(const char *cstr) {
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

#endif

#ifdef VECTOR_IMPLEMENTATION

// taken from https://github.com/TomasBorquez/base.h/blob/master/base.h. Can view licence from the repo
#define VECTOR(type, name)          \
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

// renamed
#define foreach(vector, it) for (__typeof__(*(vector).data) *(it) = (vector).data; (vector).data && (it) < (vector).data + (vector).length; (it)++)

#define vecpush(vector, value) __base_vec_push((void **)&(vector).data, &(vector).length, &(vector).capacity, sizeof(*(vector).data), &(value));
#define vecpop(vector) __base_vec_pop((vector).data, &(vector).length, sizeof(*(vector).data));
#define vecinsert(vector, value, index) __base_vec_insert((void **)&(vector).data, &(vector).length, &(vector).capacity, sizeof(*(vector).data), &(value), (index))
#define vecat(vector, index) (*(__typeof__(*(vector).data) *)__base_vec_at((void **)&(vector).data, &(vector).length, index, sizeof(*(vector).data)))
#define vecatptr(vector, index) (__base_vec_at((void **)&(vector).data, &(vector).length, (index), sizeof(*(vector).data)))
#define vecfree(vector) __base_vec_free((void **)&(vector).data, &(vector).length, &(vector).capacity)

// end taken
#endif

#endif