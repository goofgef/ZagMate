# ByteWeasel

**A tiny, modular, register-based bytecode VM in pure C**  
No AI slop today.

[![C](https://img.shields.io/badge/language-C-blue.svg?style=flat-square)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub stars](https://img.shields.io/github/stars/goofgef/ByteWeasel?style=flat-square&color=yellow)](https://github.com/goofgef/ByteWeasel/stargazers)

ByteWeasel is a lightweight, modular register-based VM with a easy-to-use API (if you know intermediate C).
ByteWeasel is all about being dynamic and flexible.

### Why register-based?
-Because register-based VMs are easier to map to machine code, are more effecient, and can be easier to optimize.

### Why did you make this?
-Because as open-source modern VMs are, like LuaVM, they arent open-source at heart. Sure, you can run it or touch its internals but your average coder probably wont be able to, say, add his own opcode. I dislike this, it feels like there is a massive closed source door for many VMs if you dont understand it. This is my attempt to fix that.

### Features
- Pure C, no major dependencies
- Register-based execution model
- Dynamic opcode handlers via function pointers in a handler table
- Variable-length instructions (operand_count + uint32_t* operands)

### Quick Example (main.c)

```c
#include "byteweasel_builtin.h"
#include "byteweasel.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

int main(){
  VM vm = {};
  init_vm(&vm, 2); //vm, capacity
  
  vm.vtable->register_handler(&vm, 0, &add); //vm, opcode, handler
  vm.vtable->register_handler(&vm, 1, &sub);
  
  Instruction* bytecode = malloc(2 * sizeof(Instruction));
  
  int64_t ops0[] = {0, 1, 2};
  int64_t ops1[] = {1, 1, 2};
  
  bytecode[0] = vm.vtable->make(0, 3, ops0); //opcode, operand_count, operands
  bytecode[1] = vm.vtable->make(1, 3, ops1);
  
  vm.regs[1].data.value = 11;
  vm.regs[2].data.value = 7;
  
  vm.vtable->write(&vm, bytecode, 2); //vm, bytecode, length
  vm.vtable->run(&vm); 
  
  printf("Result in r0: %" PRId64 "\n", vm.regs[0].data.value);
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
