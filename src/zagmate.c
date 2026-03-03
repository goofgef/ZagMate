#include "zagmate.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

int write_vm(VM *vm, Instruction* bytecode, size_t len) {
    if (!bytecode){
        printf("Bytecode is empty!\n");
        return 1;
    }
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }

    vm->bytecode = bytecode;
    vm->program_size = len;

    return 0;
}

int append_vm(VM *vm, Instruction instruction) {
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }

    if (!vm->bytecode || vm->program_size >= vm->capacity){
        printf("Cannot append to bytecode, bytecode is full!\n");
        return 1;
    }
    vm->bytecode[vm->program_size] = instruction;

    vm->program_size++;

    return 0;
}

int run_vm_cycle(VM *vm) {
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }
    if (!vm->bytecode){
        printf("Bytecode is empty!\n");
        return 1;
    }

    Instruction current_instruction = vm->bytecode[vm->pc];
    Handler handler = vm->handlers[current_instruction.opcode];
    if (!handler){
        printf("Unknown opcode %u\n", current_instruction.opcode);
        return 1;
    }

    vm->pc++;
    handler(vm, &current_instruction);
    return 0;
}

int run_vm(VM *vm) {
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }
    for (size_t i = 0; i < vm->program_size; i++) {
        if (vm->halted){
            break;
        }
        run_vm_cycle(vm);
    }
    return 0;
}

Instruction make_vm(uint8_t opcode, uint8_t operand_count, int64_t operands[]){
    Instruction instruction;
    instruction.opcode = opcode;
    instruction.operand_count = operand_count;
    instruction.operands = malloc(operand_count * sizeof(int64_t));

    if (!instruction.operands){
        printf("Memory allocation error!\n");
        return instruction;
    }

    for (size_t i = 0; i < operand_count; i++){
        instruction.operands[i] = operands[i];
    }

    return instruction;
}

int register_handler_vm(VM* vm, uint8_t opcode, Handler handler) {
    if (!handler){
        printf("Handler not found!\n");
        return 1;
    }
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }
    vm->handlers[opcode] = handler;
    return 0;
}

int clean_vm(VM *vm) {
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }

    for (size_t i = 0; i < vm->program_size; i++) {
        free(vm->bytecode[i].operands);
    }

    vm->program_size = 0;

    free(vm->bytecode);
    free(vm->vtable);

    vm->program_size = 0;
    return 0;
}

int init_vtable(vtable* vtable) {
    vtable->write = write_vm;
    vtable->run = run_vm;
    vtable->clean = clean_vm;
    vtable->register_handler = register_handler_vm;
    vtable->make = make_vm;
    return 0;
}

int init_vm(VM *vm) {
    vm->program_size = 0;
    vm->bytecode = NULL;
    vm->vtable = malloc(sizeof(vtable));
    init_vtable(vm->vtable);
    vm->capacity = 0;

    vm->pc = 0;

    vm->halted = 0;
    for (size_t i = 0; i < 31; i++){
        vm->regs[i].address = i;
        vm->regs[i].data.value = 0;
    }
    return 0;
}

Register* find_register(Register* regs, uint32_t addr, size_t count){
    return &regs[addr];
}