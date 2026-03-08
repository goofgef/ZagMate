# v1.0.4 - ByteWeasel
###### _Register based VM framework_
## **v1.0.0 - Major Release (Shining Stoat)**
_-Added CHANGELOG.md  
-Made make_vm return a ReturnStatus instead of an Instruction struct, now writes to an Instruction buffer argument  
-Tons of bug fixes, mostly just better error handling + best practices  
-Added CI (build.yml) file in .github/workflows.  
-Made vtable inline with init_vm, gave it static modifier.  
-Added comments to main.c and updated README.md code example_  

### **v1.0.1 - Minor release**
_-Added DataType to register struct  
-Added DataType checking to add & sub handler
-Added ASCII art weasel face_  

### **v1.0.2 - Minor release**
_-Stopped doing if(fread_ret_n) and fixed it to if (!fread_ret_n)_


### **v1.0.3 - Minor release**
_-Added optimized flag to config
-It is off by default and in main.c_

### **What changed on v1.0.4 - Minor release?**
_-Formatted CHANGELOG.md better  
-Added push and pop function for stack in VM struct  
-Added names for major releases that follow a [Verb][Weasel-like mammal] pattern_  

###### _Current version is v1.0.4 - Minor release_