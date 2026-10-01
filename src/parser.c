#include "bass/parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bass/base.h"
#include "bass/types.h"

DEFINE_VECTOR(string_t, operands)

static const char instsplit[] = {'\n', ';', '\0'};
static const char operandsplit[] = {',', '\0'};

bool isinstsplit(char c) {
    return strchr(instsplit, c) != NULL;
}

bool isopsplit(char c) {
    return strchr(operandsplit, c) != NULL;
}

void skip_whitespace(string_t *s, uint64_t *offset, bool *hitend) {
    while (!*hitend && *offset < s->length && isspace(s->ptr[*offset])) {
        (*offset)++;

        if (isinstsplit(s->ptr[*offset])) {
            *hitend = true;
        }
    }
}

void skip_until_newline(string_t *s, uint64_t *offset) {
    while (*offset < s->length && s->ptr[(*offset)++] != '\n');

    (*offset)--;
}

string_t *collect_symbol(string_t *s, uint64_t *offset, bool *hitend) {
    string_t *symbol = malloc(sizeof(string_t));
    assert(symbol != NULL, "instruction allocation failed");

    *symbol = stralloc(20);

    while (*offset < s->length
           && !isinstsplit(s->ptr[*offset])
           && !isopsplit(s->ptr[*offset])
           && !isspace(s->ptr[*offset])) {
        straddc(symbol, s->ptr[*offset]);

        (*offset)++;
    }

    assert(symbol->length != 0, "found ',' or ';' while looking for operand");

    if (isopsplit(s->ptr[*offset])) (*offset)++;
    else if (isinstsplit(s->ptr[*offset])) {
        (*offset)++;

        skip_until_newline(s, offset);
        *hitend = true;
    }

    return symbol;
}

uint64_t next_inst(string_t *s, uint64_t offset, instruction_t *inst) {
    string_t *mnemonic;
    string_t *operand;
    operands_t operands = {0};
    bool hitend = false;

    skip_whitespace(s, &offset, &hitend);
    if (isinstsplit(s->ptr[offset + 1])) {
        hitend = true;
    }

    if (hitend) return offset + 1;

    mnemonic = collect_symbol(s, &offset, &hitend);

    if (hitend) {
        *inst = (instruction_t){
            .mnemonic = mnemonic,
            .opcount = 0,
            .operands = NULL,
        };

        return ++offset;
    }

    while (offset < s->length) {
        skip_whitespace(s, &offset, &hitend);
        if (hitend) break;

        operand = collect_symbol(s, &offset, &hitend);

        if (operand->length > 0) {
            vecpush(operands, *operand);
        }

        if (hitend) break;
    }
    
    *inst = (instruction_t){
        .mnemonic = mnemonic,
        .opcount = operands.length,
        .operands = operands.data,
    };

    return offset;
}

