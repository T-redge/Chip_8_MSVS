#include "chip8.h"

//Loading rom into memory
bool load_rom(uint8_t* memory)
{
	const char* file_name = "rom/4flags.ch8";
	bool success = true;

	FILE* file_open = fopen(file_name, "rb");
	if (!file_open) {
		perror("File opening failed\n");
		success = false;
	}

	long long file_size = get_file_size(file_open);


	uint8_t* buffer = malloc(file_size * sizeof(uint8_t));
	while (fread(buffer, sizeof(buffer[0]), file_size, file_open) != 0)
		;

	for (long long i = 0; i < file_size; ++i)
		memory[512 + i] = buffer[i];

	free(buffer);
	fclose(file_open);
	return success;
}
long long get_file_size(FILE* file)
{
	fseek(file, sizeof(uint8_t), SEEK_END);
#ifdef WIN32
	long long tmp = _ftelli64(file);
#else 
	long long tmp = ftell(file);
#endif
	printf("Size of file is: %lld bytes\n", tmp);
	rewind(file);

	return tmp;
}

//Stack for chip8
void push(Stack* st, uint16_t new_address)
{
	if (st->top == STACK_SIZE) {
		printf("Stack Overflow\n");
	}
	st->mem_address[st->top++] = new_address;
	printf("Top of stack: %X\n", st->mem_address[st->top-1]);
}
uint16_t pop(Stack* st)
{
	if (st->top == 0) {
		printf("Stack Underflow\n");
	}
	uint16_t tmp = st->mem_address[st->top-1];
	printf("Address: %X\n", tmp);
	return tmp;
}

//chip8 instructions
uint16_t get_opcode(uint8_t* memory, uint16_t* p_c)
{
	uint16_t tmp;
	tmp = memory[*p_c] << 8 | memory[*p_c + 1];

	*p_c += 2;
	
	return tmp;
}

void opcodeEO(uint8_t display[][SCREEN_HEIGHT])
{
	for (int y = 0; y < 32; ++y)
		for (int x = 0; x < 64; ++x)
			display[x][y] = 0;
}
void opcodeANNN(uint16_t opcode, uint16_t* i_reg)
{
	uint16_t tmp;
	tmp = opcode & 0x0FFF;
	*i_reg = tmp;
}
void opcode6XNN(uint16_t opcode, uint8_t* var_reg)
{
	uint16_t vx;
	uint8_t tmp;
	vx = (opcode & 0x0F00) >> 8;
	tmp = opcode & 0x00FF;

	var_reg[vx] = tmp;
}
void opcodeDXYN(uint16_t opcode, uint8_t* var_reg, uint16_t i_reg, uint8_t display[][SCREEN_HEIGHT], uint8_t* memory)
{
	uint16_t vx, vy, rows, coord_x, coord_y, sprite_data;
	vx = (opcode & 0x0F00) >> 8;
	vy = (opcode & 0x00F0) >> 4;
	rows = opcode & 0x000F;

	coord_x = var_reg[vx & 63];
	coord_y = var_reg[vy & 31];

	var_reg[15] = 0;

	printf("vx: %X, vy: %X, rows: %X, coord_x: %X, coord_y: %X\n", vx, vy, rows, coord_x, coord_y);

	for (int i = 0; i < rows; ++i) {
		sprite_data = memory[i_reg + i];
		printf("Sprite_data: %X\t", sprite_data);

		int xpixelinv = 7;
		int xpixel = 0;

		printf("\n");
		for (xpixel = 0; xpixel < 8; ++xpixel, --xpixelinv) {
			int mask = 1 << xpixelinv;

			printf("%X\t", mask);
			if (sprite_data & mask) {
				int x = coord_x + xpixel;
				int y = coord_y + i;
				if (display[x][y] == 1)
					var_reg[15] = 1;
				display[x][y] ^= 1;
			}
		}
		printf("\n");
	}
	for (int y = 0; y < 32; ++y) {
		for (int x = 0; x < 64; ++x) {
			printf("%d", display[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}
void opcode7XNN(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx, tmp;
	vx = (opcode & 0x0F00) >> 8;
	tmp = opcode & 0x00FF;

	var_reg[vx] += tmp;

	printf("vx: %X, tmp: %X, var_reg[%X]: %X\n", vx, tmp, vx, var_reg[vx]);
}
void opcode1NNN(uint16_t opcode, uint16_t* p_c)
{
	uint16_t tmp;
	tmp = opcode & 0x0FFF;

	*p_c = tmp;
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
void opcode2NNN(uint16_t opcode, uint16_t* p_c, Stack* stack)
{
	uint16_t tmp = opcode & 0x0FFF;

	push(stack, *p_c);

	*p_c = tmp;
	printf("Jumping to: %X\n", tmp);
}
void opcodeEE(uint16_t* p_c, Stack* stack)
{
	*p_c = pop(stack);
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

	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY2(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	var_reg[vx] &= var_reg[vy];

	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY3(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	var_reg[vx] ^= var_reg[vy];

	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY4(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	uint16_t tmp = var_reg[vx] + var_reg[vy];
	if (tmp > 255)
		var_reg[15] = 1;
	else
		var_reg[15] = 0;

	var_reg[vx] += var_reg[vy];

	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY5(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	if (var_reg[vx] >= var_reg[vy])
		var_reg[15] = 1;
	else
		var_reg[15] = 0;

	var_reg[vx] -= var_reg[vy];
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XY6(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	var_reg[15] = var_reg[vx] & 0x1;

	printf("%X\n", var_reg[0xF]);

	var_reg[vx] >>= 1;



}
void opcode8XY7(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;

	printf("vx: %X, vy: %X\n", vx, vy);

	if (var_reg[vy] >= var_reg[vx])
		var_reg[15] = 1;
	else
		var_reg[15] = 0;

	var_reg[vx] = var_reg[vy] - var_reg[vx];
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
}
void opcode8XYE(uint16_t opcode, uint8_t* var_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	var_reg[15] = var_reg[vx] & 0x1;

	printf("%X\n", var_reg[0xF]);

	var_reg[vx] <<= 1;
}
void opcodeFX65(uint16_t opcode, uint8_t* var_reg, uint16_t i_reg, uint8_t* memory)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	uint16_t tmp = i_reg;

	for (uint16_t x = 0; x <= vx; ++x) {
		var_reg[x] = memory[tmp];
		printf("var_reg[%d]: %X\t", x, tmp);
		++tmp;
		printf("i_reg: %X\n", tmp);
	}
	printf("\n");
}
void opcodeFX55(uint16_t opcode, uint8_t* var_reg, uint16_t i_reg, uint8_t* memory)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	uint16_t tmp = i_reg;

	for (uint16_t x = 0; x <= vx; ++x) {
		memory[tmp] = var_reg[x];
		printf("var_reg[%d]: %X\t", x, tmp);
		++tmp;
		printf("i_reg: %X\n", tmp);
	}
	printf("\n");
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
void opcodeFX1E(uint16_t opcode, uint8_t* var_reg, uint16_t* i_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	printf("var_reg[%X]: %X\n", vx, var_reg[vx]);
	printf("I_reg: %X\n", *i_reg);

	*i_reg += var_reg[vx];

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
void opcodeFX15(uint16_t opcode, uint8_t* var_reg, uint8_t delay_timer)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	delay_timer = var_reg[vx];
}
void opcodeFX18(uint16_t opcode, uint8_t* var_reg, uint8_t sound_timer)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;

	sound_timer = var_reg[vx];
}
/*void opcodeFX29(uint16_t opcode, uint16_t *i_reg)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
}*/
/*void opcodeFX0A(uint16_t opcode, uint8_t *var_reg, uint16_t *p_c, uint8_t *keys)
{
	*p_c -= 2;

	uint8_t vx = (opcode & 0x0F00) >> 8;

}*/

void update_timers(uint8_t sound_timer, uint8_t delay_timer)
{
	if (delay_timer > 0)
		delay_timer--;
	if (sound_timer > 0)
		sound_timer--;
}