#include "byteweasel.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

ReturnStatus write_vm(VM *vm, Instruction* bytecode, size_t len) {
    if (!vm){
        return NULL_VM;
    }
    if (!bytecode){
        return NULL_BYTECODE;
    }

    vm->bytecode = bytecode;
    vm->program_size = len;

    return OK;
}

ReturnStatus append_vm(VM *vm, Instruction instruction) {
    if (!vm){
        return NULL_VM;
    }

    if (!vm->bytecode){
        return NULL_BYTECODE;
    }
    if (vm->program_size >= vm->capacity){
        return FULL_BYTECODE;
    }
    vm->bytecode[vm->program_size] = instruction;

    vm->program_size++;

    return 0;
}

ReturnStatus run_vm_cycle(VM *vm) {
    if (!vm){
        return NULL_VM;
    }
    if (!vm->bytecode){
        return NULL_BYTECODE;
    }

    Instruction current_instruction = vm->bytecode[vm->pc];
    Handler handler = vm->handlers[current_instruction.opcode];

    if (!handler){
        return GENERAL_NULL;
    }

    vm->pc++;
    int result = handler(vm, &current_instruction);
    if (result != 0){
        vm->halted = 1;
        return GENERAL_NULL;
    }
    return 0;
}

size_t find_symbol_vm(VM* vm, const char* name) {
    for (size_t i = 0; i < vm->symbol_count; i++) {
        if (strcmp(vm->symbols[i].name, name) == 0) {
            return vm->symbols[i].pc;
        }
    }
    return SIZE_MAX;
}

ReturnStatus register_symbol_vm(VM* vm, char* name, size_t pc) {
    if (!vm){
        return NULL_VM;
    }
    vm->symbols[vm->symbol_count].name = name;
    vm->symbols[vm->symbol_count].pc = pc;
    vm->symbol_count++;
    return OK;
}

ReturnStatus run_range_vm(VM *vm, size_t start, size_t end) {
    if (!vm){
        return NULL_VM;
    }

    size_t old_pc = vm->pc;
    vm->pc = start;

    while (vm->pc < end && vm->pc < vm->program_size && !vm->halted) {
        run_vm_cycle(vm);
        if (vm->pc > end){
            break;
        }
    }

    vm->pc = old_pc;
    return OK;
}

ReturnStatus run_vm(VM *vm) {
    if (!vm){
        return NULL_VM;
    }
    while (vm->pc < vm->program_size && !vm->halted) {
        run_vm_cycle(vm);
    }
    return OK;
}

Instruction make_vm(uint8_t opcode, uint8_t operand_count, int64_t operands[]){
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

ReturnStatus register_handler_vm(VM* vm, uint8_t opcode, Handler handler) {
    if (!handler){
        return GENERAL_NULL;
    }
    if (!vm){
        return NULL_VM;
    }
    vm->handlers[opcode] = handler;
    return 0;
}

ReturnStatus clean_vm(VM *vm) {
    if (!vm){
        return NULL_VM;
    }

    for (size_t i = 0; i < vm->program_size; i++) {
        free(vm->bytecode[i].operands);
    }

    vm->program_size = 0;

    free(vm->bytecode);

    vm->bytecode = NULL;

    return OK;
}

ReturnStatus reset_vm(VM* vm, size_t capacity) {
    if (!vm) {
        return NULL_VM;
    }

    for (size_t i = 0; i < vm->program_size; i++) {
        free(vm->bytecode[i].operands);
    }

    free(vm->bytecode);
    vm->bytecode = NULL;
    vm->program_size = 0;
    vm->pc = 0;
    vm->halted = 0;
    vm->sp = 0;
    vm->capacity = capacity;
    for (size_t i = 0; i < 32; i++) {
        vm->regs[i].data.value = 0;
    }
    return OK;
}

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
};

ReturnStatus init_vm(VM *vm, size_t capacity) {
    vm->program_size = 0;
    vm->bytecode = NULL;
    vm->vtable = &default_vtable;

    vm->capacity = capacity;

    vm->pc = 0;
    vm->sp = 0;

    vm->halted = 0;
    for (size_t i = 0; i < 32; i++){
        vm->regs[i].data.value = 0;
    }
    return OK;
}

Register* find_register(VM* vm, int64_t addr, size_t count){
    if (addr < 0 || (size_t)addr >= count){
        return NULL_REGISTER;
    }
    return &vm->regs[(size_t)addr];
}