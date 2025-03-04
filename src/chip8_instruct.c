#include "chip8_instruct.h"
#include "chip8_struct.h"
#include "Stack.h"

#include <stdbool.h>
#include <time.h>

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

void opcode00E0(Chip8* chip8)
{
	for (int y = 0; y < 32; ++y)
		for (int x = 0; x < 64; ++x)
			chip8->display[x][y] = 0;
	chip8->draw_flag = true;
}
void opcode00EE(Chip8* chip8, Stack* stack)
{
	chip8->p_c = pop(stack);
}
void opcode1NNN(Chip8* chip8)
{
	uint16_t tmp;
	tmp = chip8->opcode & 0x0FFF;

	chip8->p_c = tmp;
}
void opcode2NNN(Chip8* chip8, Stack* stack)
{
	uint16_t tmp = chip8->opcode & 0x0FFF;

	push(stack, chip8->p_c);

	chip8->p_c = tmp;
}
void opcode3XNN(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t tmp = chip8->opcode & 0x00FF;

	if (chip8->var_reg[vx] == tmp) {
		
		chip8->p_c += 2;

	}
}
void opcode4XNN(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t tmp = chip8->opcode & 0x00FF;

	if (chip8->var_reg[vx] != tmp) {
		
		chip8->p_c += 2;

	}
}
void opcode5XY0(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->var_reg[vx] == chip8->var_reg[vy]) {
		
		chip8->p_c += 2;

	}
}
void opcode6XNN(Chip8* chip8)
{
	uint8_t vx;
	uint8_t tmp;
	vx = (chip8->opcode & 0x0F00) >> 8;
	tmp = chip8->opcode & 0x00FF;

	chip8->var_reg[vx] = tmp;
}
void opcode7XNN(Chip8* chip8)
{
	uint8_t vx, tmp;
	vx = (chip8->opcode & 0x0F00) >> 8;
	tmp = chip8->opcode & 0x00FF;

	chip8->var_reg[vx] += tmp;
}
void opcode8XY0(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->var_reg[vx] = chip8->var_reg[vy];
}
void opcode8XY1(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->var_reg[vx] |= chip8->var_reg[vy];
	chip8->var_reg[15] = 0;
}
void opcode8XY2(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->var_reg[vx] &= chip8->var_reg[vy];
	chip8->var_reg[15] = 0;
}
void opcode8XY3(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->var_reg[vx] ^= chip8->var_reg[vy];
	chip8->var_reg[15] = 0;
	
}
void opcode8XY4(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;
	uint16_t addition = chip8->var_reg[vx] + chip8->var_reg[vy];

	chip8->var_reg[vx] += chip8->var_reg[vy];

	if (addition > 255)
		chip8->var_reg[15] = 1;
	else
		chip8->var_reg[15] = 0;
}
void opcode8XY5(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;


	uint8_t test_x = chip8->var_reg[vx];
	uint8_t test_y = chip8->var_reg[vy];
	
	chip8->var_reg[vx] -= chip8->var_reg[vy];

	if (test_x < test_y)
		chip8->var_reg[15] = 0;
	else
		chip8->var_reg[15] = 1;
	
	
	
}
void opcode8XY6(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	uint8_t shift_x = chip8->var_reg[vx] & 0x1;

	chip8->var_reg[vx] = chip8->var_reg[vy] >> 1;


	chip8->var_reg[15] = shift_x;
}
void opcode8XY7(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;


	chip8->var_reg[vx] = chip8->var_reg[vy] - chip8->var_reg[vx];

	if (chip8->var_reg[vy] < chip8->var_reg[vx])
		chip8->var_reg[15] = 0;
	else
		chip8->var_reg[15] = 1;
}
void opcode8XYE(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;
	
	uint8_t mask = (1 << 7);


	uint8_t shift_x = chip8->var_reg[vx] & mask;

	chip8->var_reg[vx] = chip8->var_reg[vy] << 1;


	if (shift_x == 0x80)
		chip8->var_reg[15] = 1;
	else
		chip8->var_reg[15] = 0;
}
void opcode9XY0(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->var_reg[vx] != chip8->var_reg[vy]) {
		chip8->p_c += 2;
	}

}
void opcodeANNN(Chip8* chip8)
{
	uint16_t tmp;
	tmp = chip8->opcode & 0x0FFF;
	chip8->i_reg = tmp;
}
void opcodeBNNN(Chip8* chip8)
{
	uint16_t tmp = (chip8->opcode & 0xFFF) + chip8->var_reg[0];

	chip8->p_c = tmp;



}
void opcodeCXNN(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t nn = chip8->opcode & 0x00FF;

	chip8->var_reg[vx] = nn & (rand() % 255);
}
void opcodeDXYN(Chip8* chip8)
{
	uint8_t vx, vy, sprite_height, x_local, y_local, pixel;
	vx = (chip8->opcode & 0x0F00) >> 8;
	vy = (chip8->opcode & 0x00F0) >> 4;
	sprite_height = chip8->opcode & 0x000F;
	x_local = chip8->var_reg[vx] % 64;
	y_local = chip8->var_reg[vy] % 32;


	chip8->var_reg[15] = 0;
	for (int y_coord = 0; y_coord < sprite_height; y_coord++) {
		pixel = chip8->memory[chip8->i_reg + y_coord];
		int y = (y_local + y_coord);
		if (y >= DISPLAY_HEIGHT)
			break;

		for (int x_coord = 0; x_coord < 8; x_coord++) {
			int mask = (0x80 >> x_coord);
			int x = (x_local + x_coord);
			if (x >= DISPLAY_WIDTH)
				break;
			if ((pixel & mask) != 0) {
				if (chip8->display[x][y] == 1)
					chip8->var_reg[15] = 1;
				chip8->display[x][y] ^= 1;
			}
		}
	}
	chip8->draw_flag = true;
}
void opcodeEX9E(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;


	if (chip8->keys[chip8->var_reg[vx]] == 1)
		chip8->p_c += 2;
}
void opcodeEXA1(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;

	if (chip8->keys[chip8->var_reg[vx]] == 0)
		chip8->p_c += 2;
}
void opcodeFX07(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->var_reg[vx] = chip8->delay_timer;
}
void opcodeFX15(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->delay_timer = chip8->var_reg[vx];
}
void opcodeFX18(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->sound_timer = chip8->var_reg[vx];
}
void opcodeFX29(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;

	switch (chip8->var_reg[vx]) {
	case 0x0:
		chip8->i_reg = chip8->memory[80];
		break;
	case 0x1:
		chip8->i_reg = chip8->memory[85];
		break;
	case 0x2:
		chip8->i_reg = chip8->memory[90];
		break;
	case 0x3:
		chip8->i_reg = chip8->memory[95];
		break;
	case 0x4:
		chip8->i_reg = chip8->memory[100];
		break;
	case 0x5:
		chip8->i_reg = chip8->memory[105];
		break;
	case 0x6:
		chip8->i_reg = chip8->memory[110];
		break;
	case 0x7:
		chip8->i_reg = chip8->memory[115];
		break;
	case 0x8:
		chip8->i_reg = chip8->memory[120];
		break;
	case 0x9:
		chip8->i_reg = chip8->memory[125];
		break;
	case 0xA:
		chip8->i_reg = chip8->memory[130];
		break;
	case 0xB:
		chip8->i_reg = chip8->memory[135];
		break;
	case 0xC:
		chip8->i_reg = chip8->memory[140];
		break;
	case 0xD:
		chip8->i_reg = chip8->memory[145];
		break;
	case 0xE:
		chip8->i_reg = chip8->memory[150];
		break;
	case 0xF:
		chip8->i_reg = chip8->memory[155];
		break;
	default:
		break;
	}
}
void opcodeFX33(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;


	uint8_t tmp_one = chip8->var_reg[vx] / (10 * 10);
	uint8_t tmp_two = (chip8->var_reg[vx] % (10 * 10)) / 10;
	uint8_t tmp_three = (chip8->var_reg[vx] % 10) / 1;

	chip8->memory[chip8->i_reg] = tmp_one;
	chip8->memory[chip8->i_reg + 1] = tmp_two;
	chip8->memory[chip8->i_reg + 2] = tmp_three;
}
void opcodeFX55(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint16_t tmp = chip8->i_reg;
	for (int i = 0; i <= vx; ++i) {
		chip8->memory[chip8->i_reg] = chip8->var_reg[i];
		++chip8->i_reg;
	}
	
	if (!chip8->old_instruction) {
		chip8->i_reg = tmp;
	}
}
void opcodeFX65(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	uint16_t tmp = chip8->i_reg;
	for (int i = 0; i <= vx; ++i) {
		chip8->var_reg[i] = chip8->memory[chip8->i_reg];
		++chip8->i_reg;
	}
	
	if (!chip8->old_instruction) {
		chip8->i_reg = tmp;
	}
}
void opcodeFX0A(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	
	bool key_press = false;
	
	for (uint8_t i = 0; i < 16; i++) {
		if (chip8->keys[i] != 0) {
			key_press = true;
			chip8->var_reg[vx] = i;
			chip8->keys[i] = 0;
			break;
		}
	}

	if (!key_press)
		chip8->p_c -= 2;

}
void opcodeFX1E(Chip8* chip8)
{
	uint8_t vx = (chip8->opcode & 0x0F00) >> 8;
	

	chip8->i_reg += chip8->var_reg[vx];

}