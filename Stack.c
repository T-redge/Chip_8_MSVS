#include "Stack.h"
//Stack for chip8
void init_stack(Stack* stack)
{
	stack->top = -1;
}

static int full_stack(Stack* stack) 
{
	return stack->top == STACK_SIZE - 1;
}
static int empty_stack(Stack* stack) 
{
	return stack->top == -1;
}
void push(Stack* stack, uint16_t new_address)
{
	if (full_stack(stack)) {
		printf("Stack Overflow\n");
		exit(EXIT_FAILURE);
	}
	stack->mem_address[++stack->top] = new_address;
	printf("Top of stack: %X\n", stack->mem_address[stack->top - 1]);
}
uint16_t pop(Stack* stack)
{
	if (empty_stack(stack)) {
		printf("Stack Underflow\n");
		exit(EXIT_FAILURE);
	}
	uint16_t tmp = stack->mem_address[stack->top--];
	printf("Address: %X\n", tmp);
	return tmp;
}
