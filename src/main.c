#include "byteweasel.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

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

    Register* reg0 = find_register(vm, instruction->operands[0], vm->config.register_count);
    Register* reg1 = find_register(vm, instruction->operands[1], vm->config.register_count);
    Register* reg2 = find_register(vm, instruction->operands[2], vm->config.register_count);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    if (reg1->type != TYPE_INT || reg2->type != TYPE_INT){
        fprintf(stderr,"Type mismatch!\n");
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

    Register* reg0 = find_register(vm, instruction->operands[0], vm->config.register_count);
    Register* reg1 = find_register(vm, instruction->operands[1], vm->config.register_count);
    Register* reg2 = find_register(vm, instruction->operands[2], vm->config.register_count);

    if (reg0 == NULL_REGISTER || reg1 == NULL_REGISTER || reg2 == NULL_REGISTER){
        fprintf(stderr,"Not enough registers!\n");
        return 1;
    }

    if (reg1->type != TYPE_INT || reg2->type != TYPE_INT){
        fprintf(stderr,"Type mismatch!\n");
        return 1;
    }
    reg0->data.value = reg1->data.value - reg2->data.value;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        fprintf(stderr, "Usage: byteweasel -w/--what\n");
        return 1;
    }

    if (argc > 1 && (strcmp(argv[1], "--what") != 0 && strcmp(argv[1], "-w") != 0)) {
        fprintf(stderr, "Usage: byteweasel -w/--what\n");
        return 1;
    }
    if (argc > 1 && (strcmp(argv[1], "--what") == 0 || strcmp(argv[1], "-w") == 0)) {
        printf("ByteWeasel is a register-based VM made in C.\n");
        printf("ByteWeasel is meant to be a framework that others can build on.\n");
        return 0;
    }else{
		//Initalize vm struct and config (parameters for init_vm basically)
        VM vm = {0};
		Config config = {
			.register_count = 64,
			.stack_size = 1024,
			.handler_count = 256,
			.symbol_size = 1024,
			.capacity = 1024,
            .optimized = 0
		};
		//Set vm.config to the config variable we declared
        vm.config = config;

		//Initalize the vm
        init_vm(&vm);

		//Register 2 custom opcodes/instructions
        vm.vtable->register_handler(&vm, 0, &add);
        vm.vtable->register_handler(&vm, 1, &sub);

		//Allocate a bytecode array
        Instruction* bytecode = malloc(2 * sizeof(Instruction));

		//Operands for 2 instructions, ADD and SUB
        int64_t ops0[] = {0, 1, 2};
        int64_t ops1[] = {1, 1, 2};

		//make takes opcode, operand_count, operands and a buffer to write to. It just creates an instruction from what you give it.
        vm.vtable->make(0, 3, ops0, &bytecode[0]);
        vm.vtable->make(1, 3, ops1, &bytecode[1]);

		//Assign 2 registers some values
        vm.regs[1].data.value = 11;
        vm.regs[2].data.value = 7;

		//Write bytecode to vm
        vm.vtable->write(&vm, bytecode, 2);

		//Prints the bytecode, table[opcode] should always equal the mnemonic of your opcode
		char* table[] = {"ADD", "SUB"};
        vm.vtable->dump(&vm, table);

		//Interpret the bytecode
        vm.vtable->run(&vm);

		//Print stored results
        printf("Result in r0: %" PRId64 "\n", vm.regs[0].data.value); //18
        printf("Result in r1: %" PRId64 "\n", vm.regs[1].data.value); //4

		//Clean up after us and init_vm
        vm.vtable->clean(&vm);
        return 0;
    }
    return 0;
}