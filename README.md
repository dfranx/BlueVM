## todo

speed up
- [ ] #2 bv_memory

more speed:
- [ ] #2 pointers (for example, local0 pointing to object on stack so when that object changes the value, the value of local0 also changes - removes the need for pushing object on stack after modifying it) / gcollector



- more:
- [ ] (last) dont push object to stack after bv_opcode_call_method
- [ ] (last) add bv_opcode_set (operator =) changes the value of variable without changing its type (calls operator= for objects)
- [ ] (last) remove ids from bv_name_list
- [ ] (last) do all TODOs and remove itoa func
- [ ] (last) bv_program_add_global
- [ ] (last)(?) remove return type from functions
- [ ] (last)(?) opcodes for debugging