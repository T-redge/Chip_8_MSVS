#ifndef CHIP_8_STRUCT_H
#define CHIP_8_STRUCT_H

#include "Stack.h"

#define MEMORY_SIZE 4096
#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64
#define REGISTER_SIZE 16
#define KEYS_SIZE 16
#define TIMER_DELAY 60
#define SOUND_DELAY 60
#define START_ADDRESS 512
#define FONT_ADDRESS 80
#define FONT_SIZE 80

static uint8_t font[FONT_SIZE] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

typedef struct chip8
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

	bool draw_flag;
}Chip8;

void init_chip8(Chip8* chip8);
void load_rom(Chip8* chip8);
uint16_t get_opcode(Chip8* chip8);
void update_timers(Chip8* chip8);

#endif//CHIP_8_STRUCT_H