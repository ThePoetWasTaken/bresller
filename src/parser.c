#include "bass/parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bass/base.h"
#include "bass/types.h"

const stmt_t end_stmt = (stmt_t) {
    .type = END_OF_STMT,
    .name = {0},
    .operands = {0}
};

tokens_t exhaust_tokens(parser_t *parser, tokens_t tokens, bool *reached_end) {
    tokens_t bunch = {0};

    while (parser->index < tokens.length) {
        token_t token = vecat(tokens, parser->index);
        parser->index++;

        if (token.type == WHITESPACE)
            continue;

        if (token.type == NEWLINE) {
            break;
        }

        if (token.type == END) {
            *reached_end = true;
            break;
        }

        vecpush(bunch, token);
    }

    return bunch;
}

stmt_t next_stmt(parser_t *parser, tokens_t tokens) {
    if (parser->reached_end) {
        return end_stmt;
    }
    
    if (tokens.length == 0) {
        return (stmt_t){0};
    }

    bool reached_end = false;
    tokens_t exhausted = exhaust_tokens(parser, tokens, &reached_end);

    while (!reached_end && exhausted.length == 0) {
        exhausted = exhaust_tokens(parser, tokens, &reached_end);
    }
    
    if (reached_end) {
        parser->reached_end = true;
        return end_stmt;
    }
}

