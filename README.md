## todo

- [x] (last) pointers (for example, local0 pointing to object on stack so when that object changes the value, the value of local0 also changes - removes the need for pushing object on stack after modifying it) / gcollector
- [x] (last) dont push object to stack after bv_opcode_call_method
- [x] (last) add bv_opcode_set (operator =) changes the value of variable without changing its type (calls operator= for objects)
- [x] (last) remove ids from bv_name_list
- [x] (last) bv_program_add_global
- [x] (last) bv_opcode_get_global_by_name, bv_opcode_empty_stack
- [x] (last) remove return type from functions
- [ ] (last) fix external method returning value
- [ ] (last) fix it so that it can compile as C++
- [ ] (last) do all TODOs and remove itoa func
- [ ] (last) opcodes for debugging (bv_opcode_debug_file, bv_opcode_debug_line_number, bv_opcode_breakpoint, bv_opcode_breakpoint_line)
- [ ] (last) ^---> function pointers: bv_breakpoint_handler and bv_error_handler
- [ ] (last) bv_function_stepper