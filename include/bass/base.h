#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define log(level, fmt, ...) printf("[%s] `%s`@%d: " fmt "\n", level, __FILE__, __LINE__, __VA_ARGS__);

#define debug(fmt, ...) log("DEBUG", fmt, __VA_ARGS__)
#define info(fmt, ...) log("INFO", fmt, __VA_ARGS__)
#define warn(fmt, ...) log("WARNING", fmt, __VA_ARGS__)
#define error(fmt, ...) log("ERROR", fmt, __VA_ARGS__)
#define fatal(fmt, ...) log("FATAL", fmt, __VA_ARGS__)

#define assert(case, explaination) if (!(case)) { error("%s", explaination); exit(1); }

// taken from https://github.com/TomasBorquez/base.h/blob/master/base.h
#define ENSURE_STRING_LITERAL(x) ("" x "")
// end taken

#define S(string) cstrasstr(ENSURE_STRING_LITERAL(string)); // inspired but not copied

typedef struct string {
    size_t capacity;
    size_t length;
    char *ptr;
} string_t;

string_t stralloc(uint64_t capacity);
void straddc(string_t *str, char c);
void straddcs(string_t *str, const char *cstr);
void stradds(string_t *str, string_t other);
void strfree(string_t str);
const char *strascstr(string_t str);
string_t cstrasstr(const char *str);

// taken from https://github.com/TomasBorquez/base.h/blob/master/base.h. Can view licence from the repo
#define DEFINE_VECTOR(type, name)          \
    typedef struct name {           \
        size_t capacity;            \
        size_t length;              \
        type *data;                 \
    } name##_t;                     \


void __base_vec_push(void **data, size_t *length, size_t *capacity, size_t element_size, void *value);
void *__base_vec_pop(void *data, size_t *length, size_t element_size);
void __base_vec_insert(void **data, size_t *length, size_t *capacity, size_t element_size, void *value, size_t index);
void *__base_vec_at(void **data, size_t *length, size_t index, size_t elementSize);
void __base_vec_free(void **data, size_t *length, size_t *capacity);

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