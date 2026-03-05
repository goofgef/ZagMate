#ifdef _WIN32
#ifdef BYTEWEASEL_EXPORTS
  #define BYTEWEASEL_API __declspec(dllexport)
#else
  #define BYTEWEASEL_API __declspec(dllimport)
#endif
#else
#define BYTEWEASEL_API __attribute__((visibility("default")))
#endif

#ifndef BYTEWEASEL_BYTEWEASEL_BUILTIN_H
#define BYTEWEASEL_BYTEWEASEL_BUILTIN_H
#include "byteweasel.h"

BYTEWEASEL_API int add(VM* vm, Instruction* instruction);
BYTEWEASEL_API int sub(VM* vm, Instruction* instruction);

#endif //BYTEWEASEL_BYTEWEASEL_BUILTIN_H