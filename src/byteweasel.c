#include "byteweasel.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define NULL_CHECK_VM(vm); \
    do { \
        if (!(vm)) return NULL_VM; \
    } while(0)

#define NULL_CHECK_BYTECODE(bytecode); \
    do { \
        if (!(bytecode)) return NULL_BYTECODE; \
    } while(0)

//Write a whole allocated list of bytecode to vm->bytecode and set program_size to len

ReturnStatus write_vm(VM *vm, Instruction* bytecode, size_t len) {
    NULL_CHECK_VM(vm);

	NULL_CHECK_BYTECODE(bytecode);

    vm->bytecode = bytecode;
    vm->program_size = len;

    return OK;
}


//Append a single instruction to the vm and update program_size by 1

ReturnStatus append_vm(VM *vm, Instruction instruction) {
    NULL_CHECK_VM(vm);

	NULL_CHECK_BYTECODE(vm->bytecode);
    if (vm->program_size >= vm->capacity){
        return FULL_BYTECODE;
    }

    //program_size will always point to an empty slot of the program, because program_size acts as a total number of instructions. So if program_size = 0,  bytecode[program_size] will set bytecode[0] to value.
    //I tried to make this make sense
    vm->bytecode[vm->program_size] = instruction;

	//Since we are appending 1 instruction, just bump program_size by 1.
    vm->program_size++;

    return 0;
}

ReturnStatus run_vm_cycle(VM *vm) {
    NULL_CHECK_VM(vm);
	NULL_CHECK_BYTECODE(vm->bytecode);

	//Fetch current instruction based off of pc
    Instruction current_instruction = vm->bytecode[vm->pc];
    //Opcode will always represent the correct handler if user uses register_handler, because we set handlers[opcode] to *func
    Handler handler = vm->handlers[current_instruction.opcode];

    if (!handler){
        return NULL_HANDLER;
    }

	//Update PC before result 
    vm->pc++;
    int result = handler(vm, &current_instruction);
    if (result != 0){
        vm->halted = 1;
        return HALTED;
    }
    return 0;
}

size_t find_symbol_vm(VM* vm, const char* name) {
	//This is 0(N), slow, but easier to read/understand
    for (size_t i = 0; i < vm->symbol_count; i++) {
        if (strcmp(vm->symbols[i].name, name) == 0) {
            return vm->symbols[i].pc;
        }
    }
    return SIZE_MAX;
}

ReturnStatus register_symbol_vm(VM* vm, char* name, size_t pc) {
    NULL_CHECK_VM(vm);

    //Set symbols[count].field to corresponding parameter
    vm->symbols[vm->symbol_count].name = name;
    vm->symbols[vm->symbol_count].pc = pc;
    vm->symbol_count++;
    return OK;
}

ReturnStatus run_range_vm(VM *vm, size_t start, size_t end) {
    NULL_CHECK_VM(vm);

    size_t old_pc = vm->pc;
    vm->pc = start;

    while (vm->pc < end && vm->pc < vm->program_size && !vm->halted) {
        run_vm_cycle(vm);
        if (vm->pc > end){
            break;
        }
    }

	//Return to original pc before calling run_range. 
    vm->pc = old_pc;
    return OK;
}

ReturnStatus run_vm(VM *vm) {
    NULL_CHECK_VM(vm);
    //Run until vm->pc < vm->program_size and not vm->halted
    while (vm->pc < vm->program_size && !vm->halted) {
        run_vm_cycle(vm);
    }
    return OK;
}

Instruction make_vm(uint16_t opcode, uint8_t operand_count, int64_t operands[]){
    Instruction instruction;
    instruction.opcode = opcode;
    instruction.operand_count = operand_count;
    instruction.operands = malloc(operand_count * sizeof(int64_t));

    if (!instruction.operands){
        instruction.operand_count = 0;
        return *NULL_INSTRUCTION_STRUCT;
    }

    for (size_t i = 0; i < operand_count; i++){
        instruction.operands[i] = operands[i];
    }

    return instruction;
}

ReturnStatus register_handler_vm(VM* vm, uint16_t opcode, Handler handler) {
    if (!handler){
        return NULL_HANDLER;
    }
    NULL_CHECK_VM(vm);
    vm->handlers[opcode] = handler;
    return 0;
}

ReturnStatus clean_vm(VM *vm) {
    NULL_CHECK_VM(vm);

    for (size_t i = 0; i < vm->program_size; i++) {
        free(vm->bytecode[i].operands);
    }

    vm->program_size = 0;

    free(vm->bytecode);
	
    vm->bytecode = NULL;

    return OK;
}

ReturnStatus reset_vm(VM* vm) {
	NULL_CHECK_VM(vm);

    for (size_t i = 0; i < vm->program_size; i++) {
        free(vm->bytecode[i].operands);
    }

	//Just init but without the vtable and handlers being nulled out/freed.
    free(vm->bytecode);
    vm->bytecode = NULL;
    vm->program_size = 0;
    vm->pc = 0;
    vm->halted = 0;
    vm->sp = 0;
    vm->capacity = vm->config.capacity;
    for (size_t i = 0; i < vm->config.register_count; i++) {
        vm->regs[i].data.value = 0;
    }
    return OK;
}

/* file format:
 * [uint32_t magic] [size_t program_size] [instructions]
 * each instruction:
 * [uint16_t opcode] [uint8_t operand_count] [int64_t operands * operand_count]
*/

//Serialize bytecode
ReturnStatus serialize_vm(VM *vm, const char *path) {
    NULL_CHECK_VM(vm);
    if (!path)     return NULL_PATH;
    NULL_CHECK_BYTECODE(vm->bytecode);

    FILE *file = fopen(path, "wb");
    if (!file){
        return NULL_FILE;
    }

    //Write magic to disk
    uint32_t magic = BW_SERIAL_MAGIC;
    fwrite(&magic, sizeof(uint32_t), 1, file);

    //Write program_size
    fwrite(&vm->program_size, sizeof(size_t), 1, file);

    // Write each instruction
    for (size_t i = 0; i < vm->program_size; i++) {
        Instruction* ins = &vm->bytecode[i];

        fwrite(&ins->opcode, sizeof(uint16_t), 1, file);
        fwrite(&ins->operand_count, sizeof(uint8_t), 1, file);

        //Write each operand manually
        if (ins->operand_count > 0 && ins->operands) {
            fwrite(ins->operands, sizeof(int64_t), ins->operand_count, file);
        }
    }

    fclose(file);
    return OK;
}

//Deserialize bytecode
ReturnStatus deserialize_vm(VM *vm, const char *path) {
    NULL_CHECK_VM(vm);
    if (!path) return NULL_PATH;

    FILE* f = fopen(path, "rb");
    if (!f){
        return NULL_FILE;
    }

    //Check magic to make sure it matches
    uint32_t magic = 0;
    fread(&magic, sizeof(uint32_t), 1, f);
    if (magic != BW_SERIAL_MAGIC) {
        fclose(f);
        return UNEQUAL_MAGIC;
    }

    // Read program size
    size_t program_size = 0;
    fread(&program_size, sizeof(size_t), 1, f);

    // Allocate bytecode array
    Instruction* bytecode = malloc(program_size * sizeof(Instruction));
    if (!bytecode) {
        fclose(f);
        return NULL_BYTECODE;
    }

    // Read each instruction
    for (size_t i = 0; i < program_size; i++) {
        Instruction* ins = &bytecode[i];

        fread(&ins->opcode, sizeof(uint16_t), 1, f);
        fread(&ins->operand_count, sizeof(uint8_t), 1, f);

        if (ins->operand_count > 0) {
            ins->operands = malloc(ins->operand_count * sizeof(int64_t));
            if (!ins->operands) {
                // Clean up already allocated instructions
                for (size_t j = 0; j < i; j++) {
                    free(bytecode[j].operands);
                }
                free(bytecode);
                fclose(f);
                return NULL_OPERANDS;
            }
            fread(ins->operands, sizeof(int64_t), ins->operand_count, f);
        } else {
            ins->operands = NULL;
        }
    }

    fclose(f);

    //write
    //i dont even know if we should call this directly from the function
    vm->vtable->write(vm, bytecode, program_size);
    return OK;
}

ReturnStatus dump_vm(VM *vm, char* table[]) {
    NULL_CHECK_VM(vm);
    printf("Bytecode dump:\n");
    for (size_t i = 0; i < vm->program_size; i++) {
		Instruction current_ins = vm->bytecode[i];
        printf("%u   %s ", i, table[current_ins.opcode]);
		for (size_t j = 0; j < current_ins.operand_count; j++) {
			printf("%d ", current_ins.operands[j]);
		}
		printf("\n");
    }
	return OK;
}

//Default vtable of function pointers
vtable default_vtable = {
    .write            = write_vm,
    .run              = run_vm,
    .clean            = clean_vm,
    .register_handler = register_handler_vm,
    .make             = make_vm,
    .reset            = reset_vm,
    .append           = append_vm,
    .run_range        = run_range_vm,
    .find_symbol      = find_symbol_vm,
    .register_symbol  = register_symbol_vm,
    .serialize        = serialize_vm,
    .deserialize      = deserialize_vm,
    .dump             = dump_vm
};

ReturnStatus init_vm(VM *vm) {
	//Set most fields to null/zero
    vm->program_size = 0;
    vm->bytecode = NULL;
    vm->vtable = &default_vtable;

    vm->capacity = vm->config.capacity;

    vm->pc = 0;
    vm->sp = 0;

    vm->halted = 0;

	vm->handlers = malloc(sizeof(Handler) * vm->config.handler_count);
	vm->regs = malloc(sizeof(Register) * vm->config.register_count);

	vm->symbols = malloc(sizeof(Symbol) * vm->config.symbol_count);
	vm->stack = malloc(sizeof(int) * vm->config.stack_size);

    //Every register at first contains 0
    for (size_t i = 0; i < vm->config.register_count; i++){
        vm->regs[i].data.value = 0;
    }
    return OK;
}


//O(1), find register

Register* find_register(VM* vm, int64_t addr, size_t count){
    if (addr < 0 || (size_t)addr >= count){
        return NULL_REGISTER;
    }
    return &vm->regs[(size_t)addr];
}

//Default config

Config default_config(){
	Config default_config_var = {
		.register_count = 32,
		.stack_size = 1024,
		.handler_count = 256,
		.symbol_count = 256,
		.capacity = 1024
	};
	return default_config_var;
}