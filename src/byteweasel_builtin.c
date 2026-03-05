#include "byteweasel_builtin.h"
#include "byteweasel.h"
#include <stdio.h>

int add(VM* vm, Instruction* instruction){
    if (!vm){
        fprintf(stderr,"VM not initialized!\n");
        return 1;
    }
    if (!instruction){
        fprintf(stderr,"Instruction is empty!\n");
        return 1;
    }

    if (instruction->operand_count < 3) {
        fprintf(stderr,"Not enough operands!\n");
        return 1;
    }

    Register* reg0 = find_register(vm, instruction->operands[0], 32);
    Register* reg1 = find_register(vm, instruction->operands[1], 32);
    Register* reg2 = find_register(vm, instruction->operands[2], 32);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    reg0->data.value = reg1->data.value + reg2->data.value;
    return 0;
}

int sub(VM* vm, Instruction* instruction){
    if (!vm){
        fprintf(stderr,"VM not initialized!\n");
        return 1;
    }
    if (!instruction){
        fprintf(stderr,"Instruction is empty!\n");
        return 1;
    }

    if (instruction->operand_count < 3) {
        fprintf(stderr,"Not enough operands!\n");
        return 1;
    }

    Register* reg0 = find_register(vm, instruction->operands[0], 32);
    Register* reg1 = find_register(vm, instruction->operands[1], 32);
    Register* reg2 = find_register(vm, instruction->operands[2], 32);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    reg0->data.value = reg1->data.value - reg2->data.value;
    return 0;
}

int multiply(VM* vm, Instruction* instruction){
    if (!vm){
        fprintf(stderr,"VM not initialized!\n");
        return 1;
    }
    if (!instruction){
        fprintf(stderr,"Instruction is empty!\n");
        return 1;
    }

    if (instruction->operand_count < 3) {
        fprintf(stderr,"Not enough operands!\n");
        return 1;
    }

    Register* reg0 = find_register(vm, instruction->operands[0], 32);
    Register* reg1 = find_register(vm, instruction->operands[1], 32);
    Register* reg2 = find_register(vm, instruction->operands[2], 32);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    reg0->data.value = reg1->data.value * reg2->data.value;
    return 0;
}

int divide(VM* vm, Instruction* instruction){
    if (!vm){
        fprintf(stderr,"VM not initialized!\n");
        return 1;
    }
    if (!instruction){
        fprintf(stderr,"Instruction is empty!\n");
        return 1;
    }

    if (instruction->operand_count < 3) {
        fprintf(stderr,"Not enough operands!\n");
        return 1;
    }

    Register* reg0 = find_register(vm, instruction->operands[0], 32);
    Register* reg1 = find_register(vm, instruction->operands[1], 32);
    Register* reg2 = find_register(vm, instruction->operands[2], 32);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    if (!reg2->data.value){
        fprintf(stderr,"Tried to divide by zero!\n");
        return 1;
    }
    int64_t result = reg1->data.value / reg2->data.value;
    reg0->data.value = result;
    return 0;
}

int mod(VM* vm, Instruction* instruction){
    if (!vm){
        fprintf(stderr,"VM not initialized!\n");
        return 1;
    }
    if (!instruction){
        fprintf(stderr,"Instruction is empty!\n");
        return 1;
    }

    if (instruction->operand_count < 3) {
        fprintf(stderr,"Not enough operands!\n");
        return 1;
    }

    Register* reg0 = find_register(vm, instruction->operands[0], 32);
    Register* reg1 = find_register(vm, instruction->operands[1], 32);
    Register* reg2 = find_register(vm, instruction->operands[2], 32);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    if (!reg2->data.value){
        fprintf(stderr,"Tried to mod by zero!\n");
        return 1;
    }
    int64_t result = reg1->data.value % reg2->data.value;
    reg0->data.value = result;
    return 0;
}