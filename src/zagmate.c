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

int run_vm(VM *vm) {
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }
    if (!vm->bytecode){
        printf("Bytecode is empty!\n");
        return 1;
    }

    // Loop through each instruction in bytecode
    for (size_t i = 0; i < vm->program_size; i++) {
        Instruction current_instruction = vm->bytecode[i];
        Handler handler = vm->handlers[current_instruction.opcode];
        if (!handler){
            printf("Unknown opcode %u\n", current_instruction.opcode);
            return 1;
        }
        handler(vm, &current_instruction);
    }
    return 0;
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

    free(vm->bytecode);
    vm->program_size = 0;
    return 0;
}

int init_vtable(vtable* vtable) {
    vtable->write = write_vm;
    vtable->run = run_vm;
    vtable->clean = clean_vm;
    vtable->register_handler = register_handler_vm;
    return 0;
}

int init_vm(VM *vm) {
    vm->program_size = 0;
    vm->bytecode = NULL;
    vm->vtable = malloc(sizeof(vtable));
    init_vtable(vm->vtable);

    for (size_t i = 0; i < 15; i++){
        vm->regs[i].address = i;
        vm->regs[i].data.value = 0;
    }
    return 0;
}

Register* find_register(Register* regs, uint32_t addr, size_t count){
     for (size_t i = 0; i < count; i++){
        if (regs[i].address == addr) return &regs[i];
     }
     return NULL;
}

int add(VM* vm, Instruction* instruction){
    if (!vm){
        printf("VM not initialized!\n");
        return 1;
    }
    if (!instruction){
        printf("Instruction is empty!\n");
        return 1;
    }

    if (instruction->operand_count < 3) {
        printf("Not enough operands!\n");
        return 1;
    }

    Register* reg0 = find_register(vm->regs, instruction->operands[0], 15);
    Register* reg1 = find_register(vm->regs, instruction->operands[1], 15);
    Register* reg2 = find_register(vm->regs, instruction->operands[2], 15);
    if (!reg0 || !reg1 || !reg2){
        printf("Not enough registers!\n");
        return 1;
    }

    reg0->data.value = reg1->data.value + reg2->data.value;
    return 0;
}

int main() {
    VM vm = {};
    init_vm(&vm);

    Instruction* bytecode = malloc(sizeof(Instruction));

    bytecode->operand_count = 4;
    bytecode->operands = malloc(sizeof(uint32_t) * bytecode->operand_count);

    int ret_reg_handler = vm.vtable->register_handler(&vm, 0, &add);

    bytecode->opcode = 0;
    bytecode->operands[0] = 0;
    bytecode->operands[1] = 1;
    bytecode->operands[2] = 2;
    bytecode->operands[3] = 0;

    vm.regs[1].data.value = 1;
    vm.regs[2].data.value = 1;

    int ret_write = vm.vtable->write(&vm, bytecode, 1);
    int ret_run = vm.vtable->run(&vm);

    printf("reg0: %" PRId32 "\n", vm.regs[0].data.value);

    vm.vtable->clean(&vm);
    free(vm.vtable);

    return 0;
}