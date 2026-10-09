#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdbool.h>

#include "base.h"
#include "types.h"
#include "lexer.h"

typedef enum exprtype {
    INVALID_EXPR = 0,
    BINOP        = 1,
    UNOP         = 2,
    INST         = 3
} exprtype_t;

typedef struct expr {
    exprtype_t type;
    token_t token;
} expr_t;


typedef struct binop {
    expr_t self;
    expr_t *left;
    expr_t *right;
} binop_t;

typedef struct unop {
    expr_t self;
    expr_t *inner;
} unop_t;

typedef enum stmttype {
    INVALID_STMT = 0,
    END_OF_STMT  = 1,
    INSTRUCTION  = 2,
    DIRECTIVE    = 3
} stmttype_t;

DEFINE_VECTOR(expr_t *, exprs)

typedef struct stmt {
    stmttype_t type;
    token_t name;
    tokens_t operands;
} stmt_t;

DEFINE_VECTOR(stmt_t, stmts)

typedef struct parser {
    uint64_t index;
    bool reached_end;
} parser_t;


stmt_t next_stmt(parser_t *parser, tokens_t tokens);

#endif