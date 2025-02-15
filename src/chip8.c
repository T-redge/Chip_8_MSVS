#include "chip8.h"

//Loading rom into memory
void load_rom(uint8_t* memory)
{
	const int8_t* file_name = "rom/quirks.ch8";
	const int8_t* file_mode = "rb";

	long long file_size;
	uint8_t* buffer;

	FILE* file_ptr = fopen(file_name, file_mode);
	if (file_ptr != NULL) {
		fseek(file_ptr, 0, SEEK_END);
		#ifdef WIN32
		file_size = _ftelli64(file);
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
			memory[512 + i] = buffer[i];
		}
	} else {
		printf("Error: File does not exits!\n");
		exit(EXIT_FAILURE);
	}
	fclose(file_ptr);
	free(buffer);
}
//chip8 instructions
uint16_t get_opcode(uint8_t* memory, uint16_t* p_c)
{
	uint16_t tmp;
	tmp = memory[*p_c] << 8 | memory[*p_c + 1];

	*p_c += 2;
	
	return tmp;
}

void opcode00E0(uint8_t display[][SCREEN_HEIGHT], bool *draw_flag)
{
	for (int y = 0; y < 32; ++y)
		for (int x = 0; x < 64; ++x)
			display[x][y] = 0;
	*draw_flag = true;
}
void opcode00EE(uint16_t* p_c, Stack *stack)
{
	*p_c = pop(stack);
}
void opcode1NNN(uint16_t opcode, uint16_t* p_c)
{
	uint16_t tmp;
	tmp = opcode & 0x0FFF;

	*p_c = tmp;
}
void opcode2NNN(uint16_t opcode, uint16_t* p_c, Stack *stack)
{
	uint16_t tmp = opcode & 0x0FFF;

	push(stack, *p_c);

	*p_c = tmp;
}
void opcode3XNN(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t tmp = opcode & 0x00FF;

	if (var_reg[vx] == tmp) {
		
		*p_c += 2;

	}
}
void opcode4XNN(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t tmp = opcode & 0x00FF;

	if (var_reg[vx] != tmp) {
		
		*p_c += 2;

	}
}
void opcode5XY0(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	if (var_reg[vx] == var_reg[vy]) {
		
		*p_c += 2;

	}
}
void opcode6XNN(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx;
	uint8_t tmp;
	vx = (opcode & 0x0F00) >> 8;
	tmp = opcode & 0x00FF;

	var_reg[vx] = tmp;
}
void opcode7XNN(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx, tmp;
	vx = (opcode & 0x0F00) >> 8;
	tmp = opcode & 0x00FF;

	var_reg[vx] += tmp;
}
void opcode8XY0(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	var_reg[vx] = var_reg[vy];
}
void opcode8XY1(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	var_reg[vx] |= var_reg[vy];
	var_reg[15] = 0;
}
void opcode8XY2(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	var_reg[vx] &= var_reg[vy];
	var_reg[15] = 0;
}
void opcode8XY3(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	var_reg[vx] ^= var_reg[vy];
	var_reg[15] = 0;
	
}
void opcode8XY4(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	uint16_t addition = var_reg[vx] + var_reg[vy];

	var_reg[vx] += var_reg[vy];

	if (addition > 255)
		var_reg[15] = 1;
	else
		var_reg[15] = 0;
}
void opcode8XY5(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;


	uint8_t test_x = var_reg[vx];
	uint8_t test_y = var_reg[vy];
	
	var_reg[vx] -= var_reg[vy];

	if (test_x < test_y)
		var_reg[15] = 0;
	else
		var_reg[15] = 1;
	
	
	
}
void opcode8XY6(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	uint8_t shift_x = var_reg[vx] & 0x1;

	var_reg[vx] = var_reg[vy] >> 1;


	var_reg[15] = shift_x;
}
void opcode8XY7(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;


	var_reg[vx] = var_reg[vy] - var_reg[vx];

	if (var_reg[vy] < var_reg[vx])
		var_reg[15] = 0;
	else
		var_reg[15] = 1;
}
void opcode8XYE(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	
	uint8_t mask = (1 << 7);


	uint8_t shift_x = var_reg[vx] & mask;

	var_reg[vx] = var_reg[vy] << 1;


	if (shift_x == 0x80)
		var_reg[15] = 1;
	else
		var_reg[15] = 0;
}
void opcode9XY0(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	if (var_reg[vx] != var_reg[vy]) {
		*p_c += 2;
	}

}
void opcodeANNN(uint16_t opcode, uint16_t* i_reg)
{
	uint16_t tmp;
	tmp = opcode & 0x0FFF;
	*i_reg = tmp;
}
void opcodeBNNN(uint16_t opcode, uint8_t* var_reg, uint16_t* p_c)
{
	uint16_t tmp = (opcode & 0xFFF) + var_reg[0];

	*p_c = tmp;



}
void opcodeCXNN(uint16_t opcode, uint8_t* var_reg, int  random_number)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t nn = opcode & 0x00FF;

	var_reg[vx] = nn & random_number;
}
void opcodeDXYN(uint16_t opcode, uint8_t* var_reg, uint16_t i_reg, uint8_t display[][SCREEN_HEIGHT], uint8_t* memory, bool *draw_flag)
{
	uint8_t vx, vy, sprite_height, x_local, y_local, pixel;
	vx = (opcode & 0x0F00) >> 8;
	vy = (opcode & 0x00F0) >> 4;
	sprite_height = opcode & 0x000F;
	x_local = var_reg[vx] % 64;
	y_local = var_reg[vy] % 32;


	var_reg[15] = 0;
	for (int y_coord = 0; y_coord < sprite_height; y_coord++) {
		pixel = memory[i_reg + y_coord];
		int y = (y_local + y_coord);
		if (y >= SCREEN_HEIGHT)
			break;

		for (int x_coord = 0; x_coord < 8; x_coord++) {
			int mask = (0x80 >> x_coord);
			int x = (x_local + x_coord);
			if (x >= SCREEN_WIDTH)
				break;
			if ((pixel & mask) != 0) {
				if (display[x][y] == 1)
					var_reg[15] = 1;
				display[x][y] ^= 1;
			}
		}
	}
	*draw_flag = true;
}
void opcodeEX9E(uint16_t opcode, uint8_t* keys, uint16_t* p_c, uint8_t *var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;


	if (keys[var_reg[vx]] == 1)
		*p_c += 2;
}
void opcodeEXA1(uint16_t opcode, uint8_t* keys, uint16_t* p_c, uint8_t *var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	if (keys[var_reg[vx]] == 0)
		*p_c += 2;
}
void opcodeFX07(uint16_t opcode, uint8_t* var_reg, uint8_t delay_timer)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	var_reg[vx] = delay_timer;
}
void opcodeFX15(uint16_t opcode, uint8_t* var_reg, uint8_t *delay_timer)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	*delay_timer = var_reg[vx];
}
void opcodeFX18(uint16_t opcode, uint8_t* var_reg, uint8_t* sound_timer)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	*sound_timer = var_reg[vx];
}
void opcodeFX29(uint16_t opcode, uint16_t i_reg, uint8_t *var_reg, uint8_t* memory)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	switch (var_reg[vx]) {
	case 0x0:
		i_reg = memory[80];
		break;
	case 0x1:
		i_reg = memory[85];
		break;
	case 0x2:
		i_reg = memory[90];
		break;
	case 0x3:
		i_reg = memory[95];
		break;
	case 0x4:
		i_reg = memory[100];
		break;
	case 0x5:
		i_reg = memory[105];
		break;
	case 0x6:
		i_reg = memory[110];
		break;
	case 0x7:
		i_reg = memory[115];
		break;
	case 0x8:
		i_reg = memory[120];
		break;
	case 0x9:
		i_reg = memory[125];
		break;
	case 0xA:
		i_reg = memory[130];
		break;
	case 0xB:
		i_reg = memory[135];
		break;
	case 0xC:
		i_reg = memory[140];
		break;
	case 0xD:
		i_reg = memory[145];
		break;
	case 0xE:
		i_reg = memory[150];
		break;
	case 0xF:
		i_reg = memory[155];
		break;
	default:
		break;
	}
}
void opcodeFX33(uint16_t opcode, uint8_t* memory, uint16_t i_reg, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;


	uint8_t tmp_one = var_reg[vx] / (10 * 10);
	uint8_t tmp_two = (var_reg[vx] % (10 * 10)) / 10;
	uint8_t tmp_three = (var_reg[vx] % 10) / 1;

	memory[i_reg] = tmp_one;
	memory[i_reg + 1] = tmp_two;
	memory[i_reg + 2] = tmp_three;
}
void opcodeFX55(uint16_t opcode, uint8_t* var_reg, uint16_t *i_reg, uint8_t* memory, bool old)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint16_t tmp = *i_reg;
	for (int i = 0; i <= vx; ++i) {
		memory[*i_reg] = var_reg[i];
		++*i_reg;
	}
	
	if (!old) {
		*i_reg = tmp;
	}
}
void opcodeFX65(uint16_t opcode, uint8_t* var_reg, uint16_t *i_reg, uint8_t* memory, bool old)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint16_t tmp = *i_reg;
	for (int i = 0; i <= vx; ++i) {
		var_reg[i] = memory[*i_reg];
		++*i_reg;
	}
	
	if (!old) {
		*i_reg = tmp;
	}
}
void opcodeFX0A(uint16_t opcode, uint8_t *var_reg, uint16_t *p_c, uint8_t *keys)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	
	bool key_press = false;
	
	for (uint8_t i = 0; i < 16; i++) {
		if (keys[i] != 0) {
			key_press = true;
			var_reg[vx] = i;
			keys[i] = 0;
			break;
		}
	}

	if (!key_press)
		*p_c -= 2;

}
void opcodeFX1E(uint16_t opcode, uint8_t* var_reg, uint16_t* i_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	

	*i_reg += var_reg[vx];

}