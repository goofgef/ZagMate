#ifdef _WIN32
  #ifdef BYTEWEASEL_EXPORTS
    #define BYTEWEASEL_API __declspec(dllexport)
  #else
    #define BYTEWEASEL_API __declspec(dllimport)
  #endif
#else
  #define BYTEWEASEL_API __attribute__((visibility("default")))
#endif

#ifndef BYTEWEASEL_BYTEWEASEL_H
#define BYTEWEASEL_BYTEWEASEL_H
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
    char* name;
    size_t pc;
} Symbol;

typedef struct {
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
    ReturnStatus (*run_range)(struct VM*, size_t, size_t);

    size_t (*find_symbol)(struct VM*, const char*);
    ReturnStatus (*register_symbol)(struct VM*, char*, size_t);
} vtable;

typedef struct VM {
	vtable *vtable;
    int halted;

    size_t program_size;
    size_t pc;
    size_t sp;
    size_t capacity;
    size_t symbol_count;

    Handler handlers[256];
    Instruction* bytecode;
    Register regs[32];

    Symbol symbols[32];
    int64_t stack[256];
} VM;

BYTEWEASEL_API ReturnStatus init_vm(VM *vm, size_t capacity);
BYTEWEASEL_API Register* find_register(VM* vm, int64_t addr, size_t count);
#endif //BYTEWEASEL_BYTEWEASEL_H