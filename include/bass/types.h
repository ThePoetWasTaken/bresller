#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "base.h"

typedef enum {
    NO_PREFIX =  0,
    EXTEND64  = 1 << 0,
    BACKWD16  = 1 << 1,
} prefixes_t;

typedef enum {
    NO_EXT    = 0,
    MODRM_REG = 1 << 0, // encodes the register used in ModR/M 'reg' field
    INOP_REG  = 1 << 2, // encodes the register used in the lower 3 bits of an opcode
} extensions_t;

typedef enum {
    BIT16 = 16,
    BIT32 = 32,
    BIT64 = 64
} opsize_t;

typedef enum {
    REGISTER  = 0,
    MEMORY    = 1,
    IMMEDIATE = 2
} optype_t;

typedef enum {
    AX  = 0,
    BX  = 1, 
    CX  = 2, 
    DX  = 3, 
    SP  = 4,
    BP  = 5,
    SI  = 6,
    DI  = 7,
    R8  = 8,
    R9  = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15
} reg_t;

typedef struct operand {
    opsize_t size;
    optype_t type;
    union {
        reg_t reg;
        uint64_t immediate;
    } value;
} operand_t;

#endif