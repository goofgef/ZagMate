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

#define NULL_REGISTER ((Register*) NULL)
#define NULL_INSTRUCTION_STRUCT ((Instruction*) NULL)

typedef enum{
    OK = 0,
    NULL_VM,
    NULL_BYTECODE,
    NULL_REG,
    NULL_STACK,
    NULL_INSTRUCTION,
    NULL_UNKNOWN_OPCODE,
    FULL_BYTECODE,
    NULL_VTABLE,
    GENERAL_NULL
}ReturnStatus;

enum RegisterType{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_PTR
};

typedef struct VM VM;
typedef struct Instruction Instruction;
typedef int (*Handler)(struct VM*, struct Instruction*);

typedef struct {
    size_t address;
    enum RegisterType type;

    union {
        void* ptr;
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
    Instruction (*make)(uint8_t, uint8_t, int64_t[]);

    ReturnStatus (*append)(struct VM*, struct Instruction);
    ReturnStatus (*write)(struct VM*, Instruction*, size_t);
    ReturnStatus (*run)(struct VM*);
    ReturnStatus (*clean)(struct VM*);
    ReturnStatus (*register_handler)(struct VM*, uint8_t, Handler);

    ReturnStatus (*reset)(struct VM*, size_t);
} vtable;

typedef struct VM {
    vtable *vtable;
    int halted;

    size_t program_size;
    size_t pc;
    size_t sp;
    size_t capacity;

    Handler handlers[256];
    Instruction* bytecode;
    Register regs[32];

    int64_t stack[256];
} VM;

ZAGMATE_API ReturnStatus init_vm(VM *vm, size_t capacity);
ZAGMATE_API Register* find_register(Register* regs, int64_t addr, size_t count);
#endif //ZAGMATE_ZAGMATE_H