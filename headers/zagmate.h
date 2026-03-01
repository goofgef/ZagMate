#ifndef ZAGMATE_ZAGMATE_H
#define ZAGMATE_ZAGMATE_H
#include <stdint.h>

/*
 *Unsigned half byte (4 bits)
*/
typedef struct{
    uint8_t value : 4;
}u_half_byte;

/*
 *Signed half byte (4 bits)
*/
typedef struct{
    int8_t value : 4;
}half_byte;

/*
 *Instruction struct,
*/
typedef struct{
    half_byte byte_count;
    uint8_t opcode;
    int32_t operands[14];
}Instruction;


#endif //ZAGMATE_ZAGMATE_H