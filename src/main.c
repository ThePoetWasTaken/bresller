#include <stdio.h>

#include "bass/base.h"

#include "bass/parser.h"
#include "bass/lexer.h"

DEFINE_VECTOR(token_t, tokens)

int main(void) {
    string_t assembly = stralloc(128);
    FILE *file = fopen("./playground/mov.asm", "r");
    char buffer[1024] = {0};

    while (fread(buffer, 1, 1024, file)) {
        straddcs(&assembly, buffer);
    }

    // info("%s", strascstr(assembly));

    uint64_t offset = 0;
    tokens_t tokens = {0};
    
    for (token_t it = next_token(assembly, &offset); it.type != END; it = next_token(assembly, &offset)) {
        vecpush(tokens, it);
    }
    

    foreach(tokens, tok) {
        info("%d: %s", tok->type, strascstr(tok->value));
    }
    
    return 0;
}