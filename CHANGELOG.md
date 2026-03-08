**v1.0.0 - ByteWeasel**

**What changed?**
-Added CHANGELOG.md
-Made make_vm return a ReturnStatus instead of an Instruction struct, now writes to an Instruction buffer argument
-Tons of bug fixes, mostly just better error handling + best practices
-Added CI (build.yml) file in .github/workflows.
-Made vtable inline with init_vm, gave it static modifier.
-Added comments to main.c and updated README.md code example