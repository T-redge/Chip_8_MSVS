/*
*	CHIP_8 Interpretor + Rendering Capabilities
*/
#include "Stack.h"
#include "chip8_struct.h"
#include "chip8_instruct.h"
#include "SDLwindow.h"

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_mixer.h>

struct _Stack {
	int top;
	uint16_t mem_address[STACK_SIZE];
};
struct _Chip8
{
	uint8_t memory[MEMORY_SIZE];
	uint8_t display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
	uint8_t var_reg[REGISTER_SIZE];
	uint8_t keys[KEYS_SIZE];
	uint8_t delay_timer;
	uint8_t sound_timer;

	uint16_t p_c;
	uint16_t i_reg;
	uint16_t opcode;

	int loop_counter;

	bool old_instruction;
	bool draw_flag;
};
struct _SDL {
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	Mix_Chunk* beep;

	SDL_Event event;

	uint32_t* pixels;

	int video_width;
	int video_height;
	int video_scale;

	bool running_flag;
};

int main(int argc, char* argv[])
{
	if (argc != 0);
	if (argv[0]);
/************************************ Init ************************************/
	Stack stack;
	init_stack(&stack);

	Chip8 chip8;
	init_chip8(&chip8);
	load_rom(&chip8);

	SDL sdl;
	init_sdl(&sdl);

/************************************ Loop ************************************/
	while (sdl.running_flag) {
		/****************************** Chip8Interpretor ******************************/
		chip8_interpreter(&chip8, &stack);

		/******************************* Event Handling *******************************/
		event_handler(&sdl, &chip8);

		/************************************ Draw ************************************/
		render(&sdl, &chip8);

		/*********************************** Timers ***********************************/
		update_timers(&chip8);

		//Delay to slow executing loop
		SDL_Delay(1);
	}
	quit(&sdl);
	return EXIT_SUCCESS;
}

