#ifndef C_STACK_H
#define C_STACK_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#include <SDL_mixer.h>


/********************************************/
/*Stack Functions
/********************************************/
#define STACK_SIZE 16
typedef struct stack
{
	int top;
	uint16_t mem_address[STACK_SIZE];
}Stack;

void init_stack(Stack* stack);
static int full_stack(Stack* stack);
static int empty_stack(Stack* stack);
void push(Stack* stack, uint16_t new_address);		//Pushes address to top of stack
uint16_t pop(Stack* stack);

#endif