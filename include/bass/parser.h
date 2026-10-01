#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include "base.h"
#include "types.h"

// Returns the position after the current instruction. 
// Puts the newly created instruction in `inst`
uint64_t next_inst(string_t *s, uint64_t offset, instruction_t *inst);

#endif