#ifndef LEXER_H
#define LEXER_H

#include "base.h"

typedef enum {
    NEWLINE        = 0, // \n
    WORD           = 1, // rax, mov
    LITERAL        = 2,
    NUMBER         = 3, // 3
    OPERATOR       = 4, // + -
    SEPARATOR      = 5, // , [
    WHITESPACE     = 6, // 
    COMMENT        = 7, // ;
    END            = 8, // EOF 
} tokentype_t;

typedef struct token {
    uint64_t offset;
    tokentype_t type;
    string_t value;
} token_t;

token_t next_token(string_t text, uint64_t *offset);

DEFINE_VECTOR(token_t, tokens)

#endif