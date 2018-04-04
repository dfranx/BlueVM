## todo
- [ ] fix all warnings
- [ ] #2 cast when using malloc
- [ ] #2 remove some appereances of char* and int
- [ ] #2 object cast overload

speed:
- [ ] #2 implement string look up table

speed up bv_stack_push and bv_stack_delete (which take more than 50% of processing time)
- [ ] #2 try linear_function_stack when calling functions in BlueVM (instead of using recursion with bv_program_call)
and/or
- [ ] #2 bv_memory

more speed:
- [ ] #2 pointers (for example, local0 pointing to object on stack so when that object changes the value, the value of local0 also changes - removes the need for pushing object on stack after modifying it)




- more:
- [ ] (last) test bv_opcode_swap
- [ ] (last) dont push object to stack after bv_opcode_call_method
- [ ] (last) bv_opcode_set (operator =) changes the value of variable without changing its type (calls operator= for objects)
- [ ] (last) remove ids from bv_name_list