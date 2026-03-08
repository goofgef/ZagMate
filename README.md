# ByteWeasel

**A tiny, modular, register-based bytecode VM framework in pure C**  
No AI slop today.

[![C](https://img.shields.io/badge/language-C-blue.svg?style=flat-square)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub stars](https://img.shields.io/github/stars/goofgef/ByteWeasel?style=flat-square&color=yellow)](https://github.com/goofgef/ByteWeasel/stargazers)

ByteWeasel is a lightweight, modular register-based VM framework with an easy-to-use API (if you know intermediate C).
ByteWeasel is all about being dynamic and flexible.

### Why register-based?
-Because register-based VMs are easier to map to machine code, are more efficient, and can be easier to optimize.

### Why did you make this?
-Because as open-source modern VMs are, like LuaVM, they aren't open-source at heart. Sure, you can run it or touch its internals but your average coder probably wont be able to, say, add his own opcode. I dislike this, it feels like there is a massive closed source door for many VMs if you dont understand it. This is my attempt to fix that.

### Features
- Pure C, no major dependencies
- Register-based execution model
- Dynamic opcode handlers via function pointers in a handler table
- Variable-length instructions (operand_count + uint32_t* operands)

### Quick Example (main.c)

```c
#include "byteweasel.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

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

    reg0->data.value = reg1->data.value - reg2->data.value;
    return 0;
}

int main(){
    VM vm = {0};
    Config config = {
        .register_count = 64,
        .stack_size = 1024,
        .handler_count = 256,
        .symbol_count = 1024,
        .capacity = 1024
    };
    vm.config = config;
    init_vm(&vm);
    
    vm.vtable->register_handler(&vm, 0, &add);
    vm.vtable->register_handler(&vm, 1, &sub);
    
    Instruction* bytecode = malloc(2 * sizeof(Instruction));
    
    int64_t ops0[] = {0, 1, 2};
    int64_t ops1[] = {1, 1, 2};
    
    bytecode[0] = vm.vtable->make(0, 3, ops0);
    bytecode[1] = vm.vtable->make(1, 3, ops1);
    
    vm.regs[1].data.value = 11;
    vm.regs[2].data.value = 7;
    
    vm.vtable->write(&vm, bytecode, 2);
    char* table[] = {"ADD", "SUB"};
    vm.vtable->dump(&vm, table);
    vm.vtable->run(&vm);
    
    printf("Result in r0: %" PRId64 "\n", vm.regs[0].data.value); //18
    printf("Result in r1: %" PRId64 "\n", vm.regs[1].data.value); //4
    
    vm.vtable->clean(&vm);
    return 0;
}
```
### Building
To build ByteWeasel, you will need GCC, Make, and CMake

### Executable

Windows:

```bash
cd util
compile
```

Linux:
```bash
cd util
chmod +x compile.sh
./compile.sh
```

### Dynamic Library

Windows:

```bash
cd util
dlib
```

Linux:

```bash
cd util
chmod +x dlib.sh
./dlib.sh
```

### Static Library

Windows:

```bash
cd util
slib
```

Linux:

```bash
cd util
chmod +x slib.sh
./slib.sh
```
