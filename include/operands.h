#ifndef OPERADNS_H
#define OPERANDS_H

typedef enum {
    NONE     = 0,
    EXTEND64 = 1 << 0,
    BACKWD16 = 1 << 1,
} prefixes_t;

typedef enum {
    NONE,
    MODRM_REG = 1 << 0, // encodes the register uxed in ModR/M 'reg' field
    INOP_REG  = 1 << 2, // encodes the register used in the lower 3 bits of an opcode
} extensions_t;

typedef enum {
    BIT16 = 16,
    BIT32 = 32,
    BIT64 = 64
} opsize_t;

typedef struct operand {

};

#endif