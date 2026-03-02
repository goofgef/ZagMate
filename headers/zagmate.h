#ifdef _WIN32
  #ifdef ZAGMATE_EXPORTS
    #define ZAGMATE_API __declspec(dllexport)
  #else
    #define ZAGMATE_API __declspec(dllimport)
  #endif
#else
  #define ZAGMATE_API __attribute__((visibility("default")))
#endif

#ifndef ZAGMATE_ZAGMATE_H
#define ZAGMATE_ZAGMATE_H
#include <stdint.h>
#include <stddef.h>

enum RegisterType{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_PTR
};
typedef struct VM VM;
typedef struct Instruction Instruction;
typedef int (*Handler)(struct VM*, struct Instruction*);

typedef struct {
    uint32_t address;
    RegisterType type;

    union {
        void* ptr
        double value_float;
        int64_t value;
        int8_t bytes[8];
    } data;
} Register;

typedef struct Instruction {
    uint8_t opcode;
    int64_t* operands;
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
    int halted;

    size_t program_size;
    size_t pc;
    size_t sp;

    Handler handlers[256];
    Instruction* bytecode;
    Register regs[16];

    int64_t stack[256];
} VM;

int init_vm(VM *vm);
Register* find_register(Register* regs, uint32_t addr, size_t count);
#endif //ZAGMATE_ZAGMATE_H