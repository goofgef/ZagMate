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
#define BW_SERIAL_MAGIC 0x42454C4C //BELL

//Globals: name_vm, VTable names: name

//This is a list of enums, each representing a return status. NULL_WHATEVER is used when WHATEVER isnt found/NULL
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
    NULL_PATH,
    FAILED_MALLOC,
    NULL_FILE,
    UNEQUAL_MAGIC,
    NULL_OPERANDS,
    NULL_HANDLER,
    HALTED,
	OUT_OF_BOUNDS,
	BAD_READ,
    GENERAL_NULL
}ReturnStatus;

typedef enum{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_PTR
}DataType;

//Declare typedefs so i can use it before we define it.
typedef struct VM VM;
typedef struct Instruction Instruction;
typedef int (*Handler)(struct VM*, struct Instruction*);

//Config struct, basically a config file but a struct, defines sizes for VM
typedef struct{
	size_t register_count;
	size_t stack_size;
	size_t handler_count;
	size_t symbol_size;
	size_t capacity;
	int optimized;
}Config;

//Like an assembly label, just a string that corresponds to a PC value
typedef struct {
    char* name;
    size_t pc;
} Symbol;

//Register, stores a value
typedef struct {
	//Type safety thing??
	DataType type;
	//Everything shared the same memory
    union {
        void* ptr;
        double value_float;
        int64_t value;
        int8_t bytes[8];
    } data;
} Register;

// 1 instruction, contains opcode, corresponds to handler, values to work on, and the count of operands
typedef struct Instruction {
    uint16_t opcode;
    int64_t* operands;
    uint8_t operand_count;
} Instruction;

typedef struct {
	//Bunch of pointers to functions with their return type and parameters
    ReturnStatus (*make)(uint16_t, uint8_t, int64_t[], Instruction*);

    ReturnStatus (*append)(struct VM*, struct Instruction);
	ReturnStatus (*append_multiple)(struct VM*, struct Instruction[], size_t);
    ReturnStatus (*write)(struct VM*, Instruction*, size_t);
    ReturnStatus (*run)(struct VM*);
    ReturnStatus (*clean)(struct VM*);
    ReturnStatus (*register_handler)(struct VM*, uint16_t, Handler);

    ReturnStatus (*reset)(struct VM*);
    ReturnStatus (*run_range)(struct VM*, size_t, size_t);

	ReturnStatus (*serialize)(struct VM*, const char*);
	ReturnStatus (*deserialize)(struct VM*, const char*);

    size_t (*find_symbol)(struct VM*, const char*);
    ReturnStatus (*register_symbol)(struct VM*, char*, size_t);
    ReturnStatus (*dump)(struct VM*, char*[]);
} vtable;

//Core VM struct, contains everything from handlers to bytecode to 
typedef struct VM {
	vtable* vtable;

	//Halted flag
    int halted;

    size_t program_size;
    size_t pc;
    size_t sp;
    size_t capacity;
    size_t symbol_count;

	Config config;
    Handler* handlers;
    Instruction* bytecode;
    Register* regs;

    Symbol* symbols;
    int64_t* stack;
} VM;

BYTEWEASEL_API ReturnStatus init_vm(VM *vm);
BYTEWEASEL_API Register* find_register(VM* vm, int64_t addr, size_t count);
BYTEWEASEL_API Config default_config();
#endif //BYTEWEASEL_BYTEWEASEL_H
