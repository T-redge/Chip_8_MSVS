#include "chip8_struct.h"
#include "chip8_instruct.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

	//Init Rand
	srand((unsigned)time(NULL));
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

void chip8_interpreter(Chip8* chip8, Stack* stack)
{
	chip8->opcode = get_opcode(chip8);
	switch (chip8->opcode & 0xF000) {
	case 0x0000:
		switch (chip8->opcode & 0xFF) {
		case 0xE0:
			opcode00E0(chip8);
			break;
		case 0xEE:
			opcode00EE(chip8, stack);
			break;
		default:
			exit(EXIT_FAILURE);
		}
		break;
	case 0x1000:
		opcode1NNN(chip8);
		break;
	case 0x2000:
		opcode2NNN(chip8, stack);
		break;
	case 0x3000:
		opcode3XNN(chip8);
		break;
	case 0x4000:
		opcode4XNN(chip8);
		break;
	case 0x5000:
		opcode5XY0(chip8);
		break;
	case 0x6000:
		opcode6XNN(chip8);
		break;
	case 0x7000:
		opcode7XNN(chip8);
		break;
	case 0x8000:
		switch (chip8->opcode & 0x000F) {
		case 0x0:
			opcode8XY0(chip8);
			break;
		case 0x1:
			opcode8XY1(chip8);
			break;
		case 0x2:
			opcode8XY2(chip8);
			break;
		case 0x3:
			opcode8XY3(chip8);
			break;
		case 0x4:
			opcode8XY4(chip8);
			break;
		case 0x5:
			opcode8XY5(chip8);
			break;
		case 0x6:
			opcode8XY6(chip8);
			break;
		case 0x7:
			opcode8XY7(chip8);
			break;
		case 0xE:
			opcode8XYE(chip8);
			break;
		default:
			printf("Opcode not recognised!\n");
			exit(EXIT_FAILURE);
		}
		break;
	case 0x9000:
		opcode9XY0(chip8);
		break;
	case 0xA000:
		opcodeANNN(chip8);
		break;
	case 0xB000:
		opcodeBNNN(chip8);
		break;
	case 0xC000:
		opcodeCXNN(chip8);
		break;
	case 0xD000:
		opcodeDXYN(chip8);
		break;
	case 0xE000:
		switch (chip8->opcode & 0xFF) {
		case 0x9E:
			opcodeEX9E(chip8);
			break;
		case 0xA1:
			opcodeEXA1(chip8);
			break;
		default:
			printf("Opcode not recognised!\n");
			exit(EXIT_FAILURE);
		}
		break;
	case 0xF000:
		switch (chip8->opcode & 0xFF) {
		case 0x07:
			opcodeFX07(chip8);
			//running = false;
			break;
		case 0x15:
			opcodeFX15(chip8);
			//running = false;
			break;
		case 0x18:
			opcodeFX18(chip8);
			break;
		case 0x29:
			opcodeFX29(chip8);
			//running = false;
			break;
		case 0x33:
			opcodeFX33(chip8);
			//running = false;
			break;
		case 0x55:
			opcodeFX55(chip8);
			//running = false;
			break;
		case 0x65:
			opcodeFX65(chip8);
			//running = false;
			break;
		case 0x0A:
			opcodeFX0A(chip8);
			//running = false;
			break;
		case 0x1E:
			opcodeFX1E(chip8);
			//running = false;
			break;
		default:
			printf("Opcode not recognised!\n");
			exit(EXIT_FAILURE);
		}
		break;
	default:
		printf("Opcode not recognised!\n");
		exit(EXIT_FAILURE);
	}
	++chip8->loop_counter;
}

void update_timers(Chip8* chip8)
{
	if (chip8->loop_counter == 8) {
		
		if (chip8->delay_timer > 0)
			--chip8->delay_timer;
		if (chip8->sound_timer > 0) {
			//play_beep();
			--chip8->sound_timer;
		}
		chip8->loop_counter = 0;
	}
}