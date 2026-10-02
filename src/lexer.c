#include "bass/lexer.h"

#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "bass/base.h"

const char separator[] = "[],.:\"'$";
const char operator[] = "+-/*";

bool issep(char c) {
    return strchr(separator, c) != NULL;
}

bool isop(char c) {
    return strchr(operator, c) != NULL;
}

bool isword(char c) {
    return isalpha(c) || c == '_';
}

bool isnumber(char c) {
    return isdigit(c);
}

bool isnewline(char c) {
    return c == '\n';
}

bool iswhitespace(char c) {
    return isspace(c) && !isnewline(c);
}

bool iscomment(char c) {
    return c == ';';
}

bool isnotquote(char c) {
    return c != '"';
}

bool isnotnewline(char c) {
    return !isnewline(c);
}

token_t exhaust(string_t text, string_t *tvalue, uint64_t *offset, bool (*ismatch)(char), tokentype_t type) {
    while (*offset < text.length && ismatch(text.ptr[*offset])) {
        straddc(tvalue, text.ptr[*offset]);

        (*offset)++;
    }

    return (token_t) {
        .value = *tvalue,
        .type = type
    };
}

token_t next_token(string_t text, uint64_t *offset) {
    string_t tvalue = stralloc(8); 

    while (*offset < text.length) {
        char current = text.ptr[*offset];

        if (current == '"') {
            token_t tok = exhaust(text, &tvalue, offset, &isnotquote, LITERAL);

            assert(tvalue.ptr[(*offset)++] != '"', "Closing \" was not found");

            return tok;
        }

        if (iscomment(current)) {
            token_t tok = exhaust(text, &tvalue, offset, &isnotnewline, COMMENT);

            assert(tvalue.ptr[(*offset)++] != '\n', "Closing \" was not found");

            return tok;
        }

        if (isword(current)) {
            return exhaust(text, &tvalue, offset, &isword, WORD);
        }

        if (iswhitespace(current)) {
            token_t tok = exhaust(text, &tvalue, offset, &iswhitespace, WHITESPACE);
            strfree(tok.value);

            tok.value = (string_t){0};
            return tok;
        }
        
        if (isnumber(current)) {
            return exhaust(text, &tvalue, offset, &isnumber, NUMBER);
        }

        if (isop(current)) {
            straddc(&tvalue, current);
            (*offset)++;

            return (token_t){.value = tvalue, .type = OPERATOR};
        }

        if (issep(current)) {
            straddc(&tvalue, current);
            (*offset)++;

            return (token_t){.value = tvalue, .type = SEPARATOR};
        }

        if (isnewline(current)) {
            (*offset)++;

            return (token_t){.value = {0}, .type = NEWLINE};
        }
    }

    return (token_t) {
        .value = {0},
        .type  = END
    };
}