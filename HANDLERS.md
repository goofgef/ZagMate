# ByteWeasel: Handlers, how to make them

In ByteWeasel, there's something called **handlers**. Handlers live in the handler table, a large table of function pointers. 
A handler, at its core, is just a function pointer living inside a table of function pointers.

Every opcode you register in ByteWeasel corresponds to a handler.

**How does an opcode correspond to a handler?**
When you register an opcode, it just sets the function pointer at opcode in the table (so handlers[opcode]) to such handler. When ByteWeasel runs the bytecode, 
it looks up opcode in the handler table and calls such handler based on the opcode.

**How do you write a handler?**
To define a handler first in ByteWeasel, it must:
-Return an integer
-Take a pointer to the vm (&vm) and a pointer to the Instruction it corresponds to (&Instruction)

A basic handler, such as an add handler, would:
-Take in register addresses as operands
-Write the result to a specific register address

A basic handler like this can be found in main.c:
```c
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

    reg0->data.value = reg1->data.value + reg2->data.value;
    return 0;
}
```

It takes 3 registers as operands, fetches the registers from the addresses/operands (reg0, reg1, reg2), and writes
reg1 value + reg2 value to reg0.
Most arithmetic-like handlers will look similar to this, but at the core, most handlers just operate on operands/the values from the addresses in the operands
and write the result somewhere or do its function.
