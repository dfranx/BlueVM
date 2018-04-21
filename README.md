## todo

speed up bv_stack_push and bv_stack_delete (which take more than 50% of processing time)
- [ ] #2 try linear_function_stack when calling functions in BlueVM (instead of using recursion with bv_program_call)
and/or
- [ ] #2 bv_memory

more speed:
- [ ] #2 pointers (for example, local0 pointing to object on stack so when that object changes the value, the value of local0 also changes - removes the need for pushing object on stack after modifying it) / gcollector



- more:
- [ ] (last) test bv_opcode_swap
- [ ] (last) dont push object to stack after bv_opcode_call_method
- [ ] (last) add bv_opcode_set (operator =) changes the value of variable without changing its type (calls operator= for objects)
- [ ] (last) remove ids from bv_name_list
- [ ] (last) add BoolOr || and BoolAnd &&
- [ ] (last) add opcode scope_start { and scope_end } -> variable scopes
- [ ] (last) do all TODOs and remove itoa func
- [ ] (last)(?) remove return type from functions
- [ ] (last)(?) opcodes for debugging