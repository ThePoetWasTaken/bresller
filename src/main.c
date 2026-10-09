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
    token_t token = next_token(assembly, &offset);
    vecpush(tokens, token);

    while (token.type != END) {
        token = next_token(assembly, &offset);
        info("%s, %d", strascstr(token.value), token.type);
        vecpush(tokens, token);
    }

    stmts_t statements = {0};
    parser_t parser = {
        .index = 0,
        .reached_end = false
    };
    stmt_t stmt = next_stmt(&parser, tokens);
    vecpush(statements, stmt);

    while (stmt.type != END_OF_STMT) {
        stmt = next_stmt(&parser, tokens);

        vecpush(statements, stmt);
    }

    foreach(statements, it) {
        printf("%s [%ld]: ", strascstr(it->name.value), it->operands.length);
        foreach(it->operands, expr) {
            printf("%s ", strascstr(expr->value));
        }

        printf("\n");
    }

    foreach(tokens, tok) {
        strfree(tok->value);
    }

    vecfree(tokens);
    strfree(assembly);
    
    return 0;
}