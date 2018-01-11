#include <BlueVM/bv_stack.h>
#include <stdlib.h>

bv_stack bv_stack_create()
{
	bv_stack ret;
	ret.length = 0;
	ret.data = 0;
	return ret;
}
void bv_stack_delete(bv_stack* stack)
{
	for (u32 i = 0; i < stack->length; i++)
		bv_variable_deinitialize(&stack->data[i]);
	free(stack->data);
	stack->length = 0;
}


bv_variable bv_stack_penultimate(bv_stack* stack)
{
	if (stack->length < 2)
		return bv_variable_create_int(0);

	return stack->data[stack->length - 2];
}
bv_variable bv_stack_top(bv_stack* stack)
{
	return stack->data[stack->length - 1];
}
void bv_stack_pop(bv_stack* stack)
{
	bv_variable_deinitialize(&stack->data[stack->length - 1]);

	stack->length--;
	stack->data = realloc(stack->data, stack->length * sizeof(bv_variable));
}

void bv_stack_push(bv_stack* stack, bv_variable var)
{
	stack->length++;
	stack->data = realloc(stack->data, stack->length * sizeof(bv_variable));

	stack->data[stack->length - 1] = var;
}
