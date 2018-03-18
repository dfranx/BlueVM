## TODO
- [x] const_get
- [x] other opcodes
- [x] local variables (get/set_local)
- [x] global variables (get/set_global)
- [x] arrays
- [x] opcode_is_type_of
- [x] function arguments
- [x] opcode_call (calls a function)
- [x] if
- [x] call C functions
- [x] pass C variables
- [x] goto
- [x] remove all memory leaks (related to new object structures)
- [x] have a seperate function for executing each opcode (for example: bv_execute_add)
- [x] objects
- [ ] import other bv_programs into current one (call functions from another file)
- [x] #2 object constructors
- [ ] #2 bv_variable_equal, bv_variable_greater_than, bv_variable_cast, bv_variable_operator_add, etc.. + object operators
- [ ] #2 new_array [dim] -> push array on stack
- [ ] #2 more control over arrays - pushing arrays on stack, copying, global arrays, etc...
- [ ] #2 change order of subtract, add, etc operations... (for example, subtract var2 - var1, not var1-var2)
- [ ] #2 typedef bv_stack bv_vector;
- [ ] #2 use bv_name_list to store function names
- [ ] #2 fix all warnings
- [ ] #2 better casting (set object value to array variable, etc... => bv_variable_set(var1, var2))
- [ ] #2 fix bv_stack_pop in opcode calls (remove bv_variable_copy(bv_stack_top()) or put bv_stack_pop on proper lines)
- [ ] #2 better var copying (for example, copying bv_array will copy pointer address not data)
- [ ] + external bv_object & external bv_object methods
- [ ] + make const and static last 2 bits in bv_type ===> 0bCSXXXXXX C-> const, S-> static, X-> any
- [ ] + remove types from argument list
- [ ] + cast when using malloc
- [ ] + pack external_* in seperate struct

speed up bv_stack_push and bv_stack_delete (which take 50% of processing time)
- [ ] + try linear_function_stack when calling functions in BlueVm (instead of using recursion with bv_program_call)
and/or
- [ ] + bv_memory

lower down the file size:
- [ ] + implement string look up table

to gain more speed:
- [ ] !!! pointers (for example, local0 pointing to object on stack so when that object changes the value of local0 also changes - removes the need for pushing object on stack after modifying it)
BVM will push object on stack after modifying it with function which causes bugs... int x = Animal().status() will cause bugs unless we first pop the value from stack