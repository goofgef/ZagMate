#include "zagmate_builtin.h"
#include "zagmate.h"
#include <stdio.h>

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

    Register* reg0 = find_register(vm->regs, instruction->operands[0], 31);
    Register* reg1 = find_register(vm->regs, instruction->operands[1], 31);
    Register* reg2 = find_register(vm->regs, instruction->operands[2], 31);

    if (*reg0 == NULL_REGISTER || *reg1 == NULL_REGISTER || *reg2 == NULL_REGISTER){
        printf("Not enough registers!\n");
        return 1;
    }

    reg0->data.value = reg1->data.value + reg2->data.value;
    return 0;
}

int sub(VM* vm, Instruction* instruction){
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

    Register* reg0 = find_register(vm->regs, instruction->operands[0], 31);
    Register* reg1 = find_register(vm->regs, instruction->operands[1], 31);
    Register* reg2 = find_register(vm->regs, instruction->operands[2], 31);

    if (*reg0 == NULL_REGISTER || *reg1 == NULL_REGISTER || *reg2 == NULL_REGISTER){
        printf("Not enough registers!\n");
        return 1;
    }

    reg0->data.value = reg1->data.value - reg2->data.value;
    return 0;
}