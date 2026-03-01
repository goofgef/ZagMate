#ifndef ZAGMATE_ZAGMATE_H
#define ZAGMATE_ZAGMATE_H
#include <stdint.h>

typedef struct{
    uint8_t value : 4;
}u_half_byte;

typedef struct{
    int8_t value : 4;
}half_byte;

typedef struct{
    uint8_t opcode;
    half_byte byte_count;
    int32_t operands[14];
}Instruction;


#endif //ZAGMATE_ZAGMATE_H