#include "chip8_struct.h"

#include <stdio.h>

//Initialises chip8
void init_chip8(Chip8* chip8)
{
	chip8->delay_timer = TIMER_DELAY;
	chip8->sound_timer = SOUND_DELAY;

	chip8->p_c = START_ADDRESS;
	chip8->i_reg = 0;
	chip8->opcode = 0;

	chip8->loop_counter = 0;

	chip8->old_instruction = true;
	chip8->draw_flag = false;

	//Load font into memory
	for (int i = 0; i < 80; ++i)
		chip8->memory[FONT_ADDRESS + i] = font[i];
	//Init registers
	for (int i = 0; i < 16; i++)
		chip8->var_reg[i] = 0;
	//Init display
	for (int i = 0; i < DISPLAY_HEIGHT; ++i)
		for (int j = 0; j < DISPLAY_WIDTH; ++j)
			chip8->display[j][i] = 0;
	//Init keys
	for (int i = 0; i < 16; i++)
		chip8->keys[i] = 0;
}

//Loads chip8 file into memory
void load_rom(Chip8* chip8)
{
	const int8_t* file_name = "rom/br8kout.ch8";
	const int8_t* file_mode = "rb";

	long long file_size;
	uint8_t* buffer;

	FILE* file_ptr = fopen(file_name, file_mode);
	if (file_ptr != NULL) {
		fseek(file_ptr, 0, SEEK_END);
#ifdef WIN32
		file_size = _ftelli64(file_ptr);
#else
		file_size = ftell(file_ptr);
#endif		
		rewind(file_ptr);

		buffer = (uint8_t*)malloc(sizeof(uint8_t) * file_size);
		if (buffer == NULL) {
			printf("Out of memory!\n");
			exit(EXIT_FAILURE);
		}

		fread(buffer, sizeof(uint8_t), file_size, file_ptr);

		for (int i = 0; i < file_size; i++) {
			chip8->memory[START_ADDRESS + i] = buffer[i];
		}
	}
	else {
		printf("Error: File does not exits!\n");
		exit(EXIT_FAILURE);
	}
	fclose(file_ptr);
	free(buffer);
}

//Getting opcode from memory
uint16_t get_opcode(Chip8* chip8)
{
	uint16_t tmp;
	tmp = chip8->memory[chip8->p_c] << 8 | chip8->memory[chip8->p_c + 1];

	chip8->p_c += 2;

	return tmp;
}

void update_timers(Chip8* chip8)
{
	if (chip8->delay_timer > 0)
		--chip8->delay_timer;
	if (chip8->sound_timer > 0) {
		//play_beep();
		--chip8->sound_timer;
	}
}