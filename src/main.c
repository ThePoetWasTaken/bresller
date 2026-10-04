#include <stdio.h>

#include "bass/base.h"

#include "bass/parser.h"
#include "bass/lexer.h"

int main(void) {
    string_t assembly = stralloc(128);
    FILE *file = fopen("./playground/mov.asm", "r");
    char buffer[1024] = {0};
    
    while (fread(buffer, 1, 1024, file)) {
        straddcs(&assembly, buffer);
    }
    
    fclose(file);

    uint64_t offset = 0;
    tokens_t tokens = {0};
    token_t it = next_token(assembly, &offset);
    vecpush(tokens, it);

    while (it.type != END) {
        it = next_token(assembly, &offset);

        vecpush(tokens, it);
    }


    foreach(tokens, tok) {
        if (tok->type == WHITESPACE || tok->type == NEWLINE) continue;
        info("@%ld %d: %s", tok->offset, tok->type, strascstr(tok->value));
        strfree(tok->value);
    }

    vecfree(tokens);
    strfree(assembly);
    
    return 0;
}