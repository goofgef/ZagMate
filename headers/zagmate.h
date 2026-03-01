#ifndef ZAGMATE_ZAGMATE_H
#define ZAGMATE_ZAGMATE_H
#include <stdint.h>
#include <stddef.h>

typedef struct VM VM;
typedef struct Instruction Instruction;
typedef int (*Handler)(struct VM*, struct Instruction*);

typedef struct {
    uint32_t address;
    union {
        int32_t value;
        int8_t bytes[4];
    } data;
} Register;

typedef struct Instruction {
    uint8_t opcode;
    uint32_t* operands;
    uint8_t operand_count;
} Instruction;

typedef struct {
    int (*write)(struct VM*, Instruction*, size_t);
    int (*run)(struct VM*);
    int (*clean)(struct VM*);
    int (*register_handler)(struct VM*, uint8_t, Handler);
} vtable;

typedef struct VM {
    vtable* vtable;

    size_t program_size;
    size_t pc;

    Handler handlers[256];
    Instruction* bytecode;
    Register regs[16];
} VM;

int init_vm(VM *vm);
Register* find_register(Register* regs, uint32_t addr, size_t count);
#endif //ZAGMATE_ZAGMATE_H