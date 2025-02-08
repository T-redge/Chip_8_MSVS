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
	printf("Jumping to: %X\n", tmp);
}
void opcode3XNN(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t tmp = opcode & 0x00FF;

	if (var_reg[vx] == tmp) {
		printf("Skipping\n");
		*p_c += 2;

	}

	printf("vx: %X\n", vx);
	printf("tmp: %X\n", tmp);
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode4XNN(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t tmp = opcode & 0x00FF;

	if (var_reg[vx] != tmp) {
		printf("Skipping\n");
		*p_c += 2;

	}

	printf("vx: %X\n", vx);
	printf("tmp: %X\n", tmp);
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode5XY0(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	if (var_reg[vx] == var_reg[vy]) {
		printf("Skipping\n");
		*p_c += 2;

	}

	printf("vx: %X\n", vx);
	printf("vy: %X\n", vy);
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
	printf("var_reg[%X]: %X\n", vy, var_reg[vy]);
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

	printf("vx: %X, tmp: %X, var_reg[%X]: %X\n", vx, tmp, vx, var_reg[vx]);
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

	printf("vx: %X, vy: %X\n", vx, vy);

	var_reg[vx] |= var_reg[vy];
	var_reg[15] = 0;
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY2(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	var_reg[vx] &= var_reg[vy];
	var_reg[15] = 0;
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY3(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	var_reg[vx] ^= var_reg[vy];
	var_reg[15] = 0;
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
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

	printf("Var_reg[%X]: %X Var_reg[%X]: %X\n", vx, var_reg[vx], vy, var_reg[vy]);

	uint8_t test_x = var_reg[vx];
	uint8_t test_y = var_reg[vy];
	
	var_reg[vx] -= var_reg[vy];

	if (test_x < test_y)
		var_reg[15] = 0;
	else
		var_reg[15] = 1;
	
	
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
	
}
void opcode8XY6(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	

	uint8_t shift_x = var_reg[vx] & 0x1;
	printf("%X\n", var_reg[vx]);
	printf("%X\n", shift_x);

	var_reg[vx] >>= 1;

	printf("%X\n", var_reg[vx]);
	printf("%X\n", var_reg[vx] & 0x1);

	var_reg[15] = shift_x;
	printf("%X", var_reg[15]);
}
void opcode8XY7(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	var_reg[vx] = var_reg[vy] - var_reg[vx];

	if (var_reg[vy] < var_reg[vx])
		var_reg[15] = 0;
	else
		var_reg[15] = 1;
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XYE(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	
	uint8_t mask = (1 << 7);

	printf("%X\n", mask);

	uint8_t shift_x = var_reg[vx] & mask;
	printf("%X\n", var_reg[vx]);
	printf("%X\n", shift_x);

	var_reg[vx] <<= 1;

	printf("%X\n", var_reg[vx]);
	printf("%X\n", var_reg[vx] & mask);

	if (shift_x == 0x80)
		var_reg[15] = 1;
	else
		var_reg[15] = 0;
	printf("%X", var_reg[15]);
}
void opcode9XY0(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	if (var_reg[vx] != var_reg[vy]) {
		printf("Skipping\n");
		*p_c += 2;
	}

	printf("vx: %X\n", vx);
	printf("vy: %X\n", vy);
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
	printf("var_reg[%X]: %X\n", vy, var_reg[vy]);
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

	printf("tmp: %X\n", tmp);


}
void opcodeCXNN(uint16_t opcode, uint8_t* var_reg, int  random_number)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t nn = opcode & 0x00FF;

	var_reg[vx] = nn & random_number;
}
void opcodeDXYN(uint16_t opcode, uint8_t* var_reg, uint16_t i_reg, uint8_t display[][SCREEN_HEIGHT], uint8_t* memory, bool *draw_flag)
{
	uint16_t vx, vy, rows, coord_x, coord_y, sprite_data;
	vx = (opcode & 0x0F00) >> 8;
	vy = (opcode & 0x00F0) >> 4;
	rows = opcode & 0x000F;
	coord_x = var_reg[vx];
	coord_y = var_reg[vy];

	var_reg[15] = 0;
	printf("vx: %X, vy: %X, rows: %X, coord_x: %X, coord_y: %X\n", vx, vy, rows, coord_x, coord_y);

	for (int i = 0; i < rows; ++i) {
		sprite_data = memory[i_reg + i];
		printf("Sprite_data: %X\t", sprite_data);

		int xpixelinv = 7;
		int xpixel = 0;

		printf("\n");
		for (xpixel = 0; xpixel < 8; ++xpixel, --xpixelinv) {
			int mask = (0x80 >> xpixel);

			printf("%X\t", mask);
			if ((sprite_data & mask)) {
				int x = (coord_x + xpixel);
				int y = (coord_y + i);
				if (display[x][y] == 1)
					var_reg[15] = 1;
				display[x][y] ^= 1;
			}
		}
		printf("\n");
	}
	*draw_flag = true;
	printf("\n");
}
void opcodeEX9E(uint16_t opcode, uint8_t* keys, uint16_t* p_c)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	if (keys[vx] == 1) {
		*p_c += 2;
	}
}
void opcodeEXA1(uint16_t opcode, uint8_t* keys, uint16_t* p_c)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	if (keys[vx] == 0) {
		*p_c += 2;
	}
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
void opcodeFX18(uint16_t opcode, uint8_t* var_reg, uint8_t sound_timer)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	sound_timer = var_reg[vx];
}
void opcodeFX29(uint16_t opcode, uint16_t *i_reg, uint8_t *var_reg, uint8_t* memory)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	printf("Memory: %X Ireg: %d Var_reg: %X\n", memory[512], *i_reg, var_reg[vx]);

	switch (var_reg[vx]) {
	case 0x0:
		break;
	case 0x1:
		break;
	case 0x2:
		break;
	case 0x3:
		break;
	case 0x4:
		break;
	case 0x5:
		break;
	case 0x6:
		break;
	case 0x7:
		break;
	case 0x8:
		break;
	case 0x9:
		break;
	case 0xA:
		break;
	case 0xB:
		break;
	case 0xC:
		break;
	case 0xD:
		break;
	case 0xE:
		break;
	case 0xF:
		break;
	default:
		break;
	}
}
void opcodeFX33(uint16_t opcode, uint8_t* memory, uint16_t i_reg, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	printf("var_reg[%X]: %d\n", vx, var_reg[vx]);


	uint8_t tmp_one = var_reg[vx] / (10 * 10);
	uint8_t tmp_two = (var_reg[vx] % (10 * 10)) / 10;
	uint8_t tmp_three = (var_reg[vx] % 10) / 1;
	printf("Tmp_one: %d\n", tmp_one);
	printf("Tmp_two: %d\n", tmp_two);
	printf("Tmp_three: %d\n", tmp_three);

	memory[i_reg] = tmp_one;
	memory[i_reg + 1] = tmp_two;
	memory[i_reg + 2] = tmp_three;
}
void opcodeFX55(uint16_t opcode, uint8_t* var_reg, uint16_t *i_reg, uint8_t* memory, bool old)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	printf("Vx: %X\n", vx);
	uint16_t tmp = *i_reg;
	printf("tmp: %X\n", tmp);
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
	printf("tmp: %X\n", tmp);
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

	if (keys[0x0] == 1)
		var_reg[vx] = 0x0;
	else if (keys[0x1] == 1)
		var_reg[vx] = 0x1;
	else if (keys[0x2] == 1)
		var_reg[vx] = 0x2;
	else if (keys[0x3] == 1)
		var_reg[vx] = 0x3;
	else if (keys[0x4] == 1)
		var_reg[vx] = 0x4;
	else if (keys[0x5] == 1)
		var_reg[vx] = 0x5;
	else if (keys[0x6] == 1)
		var_reg[vx] = 0x6;
	else if (keys[0x7] == 1)
		var_reg[vx] = 0x7;
	else if (keys[0x8] == 1)
		var_reg[vx] = 0x8;
	else if (keys[0x9] == 1)
		var_reg[vx] = 0x9;
	else if (keys[0xA] == 1)
		var_reg[vx] = 0xA;
	else if (keys[0xB] == 1)
		var_reg[vx] = 0xB;
	else if (keys[0xC] == 1)
		var_reg[vx] = 0xC;
	else if (keys[0xD] == 1)
		var_reg[vx] = 0xD;
	else if (keys[0xE] == 1)
		var_reg[vx] = 0xE;
	else if (keys[0xF] == 1)
		var_reg[vx] = 0xF;
	else
		*p_c -= 2;
}
void opcodeFX1E(uint16_t opcode, uint8_t* var_reg, uint16_t* i_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
	

	*i_reg += var_reg[vx];
	printf("I_reg: %X\n", *i_reg);

}