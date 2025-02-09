/*
*	CHIP_8 Interpretor + Rendering Capabilities
*/
#include "SDLwindow.h"

#define MEMSIZE 4096
#define MEMFONT 80

int main(int argc, char* argv[])
{
	if (argc != 0);
	if (argv[0]);
	uint8_t memory[MEMSIZE];
	uint8_t display[SCREEN_WIDTH][SCREEN_HEIGHT] = { 0 };
	uint8_t var_reg[16] = { 0 };
	uint8_t keys[16] = {0};
	uint8_t delay_timer;
	uint8_t sound_timer;
	
	uint16_t p_c;
	uint16_t i_reg;
	uint16_t opcode;

	uint8_t font[80] = {
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

	uint32_t *pixels = malloc((SCREEN_HEIGHT * SCREEN_WIDTH) * sizeof(uint32_t));
	int pitch;
	bool old = true;
	bool draw_flag = true;
	if (!init()) {
		printf("Failed to initialize SDL!\n");
		return EXIT_FAILURE;
	}
	/*******************************************/
	/*		 Loading Rom		   */
	/*******************************************/
	load_rom(memory);
	/*******************************************/
	/*       Initialisation of variables       */
	/*******************************************/
	//Load font into memory
	for (int i = 0; i < 80; ++i)
		memory[MEMFONT + i] = font[i];
	//Init registers
	for (int i = 0; i < 16; i++)
		var_reg[i] = 0;
	//Init Display
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
		for (int j = 0; j < SCREEN_WIDTH; ++j)
			display[j][i] = 0;

	//Init Rand
	srand((unsigned)time(NULL));
	int rand_var = (rand() % (255 - 0 + 1)) + 0;

	//Init Stack
	Stack stack;
	init_stack(&stack);

	//Init Clock
	//clock_t last_loop_time = 0;
	//clock_t current_time = 0;
	int loop_count = 0;


	//Init rest
	delay_timer = 60;
	sound_timer = 60;
	p_c = 512;
	i_reg = 0;
	opcode = 0;
	pitch = 0;

	printf("\n");
	/*******************************************/
	/*		Program Loop               */
	/*******************************************/
	bool running = true;
	SDL_Event e;
	
	//current_time = clock();
	while (running) {
		//double dt = ((double)(current_time - last_loop_time)) / CLOCKS_PER_SEC;
		
		
		event_handler(&e, &running, &keys);
		/*******************************************/
		/*	    Decoding Chip 8 Opcodes        */
		/*******************************************/
		printf("\n");
		printf("I_reg: %X\n", i_reg);
		printf("p_c: %d\n", p_c);
		int j = 0;
		for (int i = 0; i < 16; i++) {
				printf("Var_reg[%X]: %X\t", i, var_reg[i]);
				++j;
				if (j % 5 == 0)
					printf("\n");
			}

		printf("\n");
		opcode = get_opcode(memory, &p_c);
		printf("Opcode: %X\n", opcode);
		switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0xFF) {
				case 0xE0:
					opcode00E0(display, &draw_flag);
					break;
				case 0xEE:
					opcode00EE(&p_c, &stack);
					break;
				default:
					printf("Opcode not recognised!\n");
					return EXIT_FAILURE;
				}
			break;
		case 0x1000:
			opcode1NNN(opcode, &p_c);
			break;
		case 0x2000:
			opcode2NNN(opcode, &p_c, &stack);
			break;
		case 0x3000:
			opcode3XNN(opcode, &p_c, var_reg);
			break;
		case 0x4000:
			opcode4XNN(opcode, &p_c, var_reg);
			break;
		case 0x5000:
			opcode5XY0(opcode, &p_c, var_reg);
			break;
		case 0x6000:
			opcode6XNN(opcode, var_reg);
			break;
		case 0x7000:
			opcode7XNN(opcode, var_reg);
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
			case 0x0:
				opcode8XY0(opcode, var_reg);
				break;
			case 0x1:
				opcode8XY1(opcode, var_reg);
				break;
			case 0x2:
				opcode8XY2(opcode, var_reg);
				break;
			case 0x3:
				opcode8XY3(opcode, var_reg);
				break;
			case 0x4:
				opcode8XY4(opcode, var_reg);
				break;
			case 0x5:
				opcode8XY5(opcode, var_reg);
				break;
			case 0x6:
				opcode8XY6(opcode, var_reg);
				break;
			case 0x7:
				opcode8XY7(opcode, var_reg);
				break;
			case 0xE:
				opcode8XYE(opcode, var_reg);
				break;
			default:
				printf("Opcode not recognised!\n");
				return EXIT_FAILURE;
			}
			break;
		case 0x9000:
			opcode9XY0(opcode, &p_c, var_reg);
			break;
		case 0xA000:
			opcodeANNN(opcode, &i_reg);
			break;
		case 0xB000:
			opcodeBNNN(opcode, var_reg, &p_c);
			break;
		case 0xC000:
			opcodeCXNN(opcode, var_reg, rand_var);
			break;
		case 0xD000:
			opcodeDXYN(opcode, var_reg, i_reg, display, memory, &draw_flag);
			break;
		case 0xE000:
			switch (opcode & 0xFF) {
			case 0x9E:
				opcodeEX9E(opcode, &keys, &p_c, var_reg);
				break;
			case 0xA1:
				opcodeEXA1(opcode, &keys, &p_c, var_reg);
				break;
			default:
				printf("Opcode not recognised!\n");
				return EXIT_FAILURE;
			}
			break;
		case 0xF000:
			switch (opcode & 0xFF) {
			case 0x07:
				opcodeFX07(opcode, var_reg, delay_timer);
				//running = false;
				break;
			case 0x15:
				opcodeFX15(opcode, var_reg, &delay_timer);
				//running = false;
				break;
			case 0x29:
				opcodeFX29(opcode, &i_reg, var_reg, memory);
				//running = false;
				break;
			case 0x33:
				opcodeFX33(opcode, memory, i_reg, var_reg);
				//running = false;
				break;
			case 0x55:
				opcodeFX55(opcode, var_reg, &i_reg, memory, old);
				//running = false;
				break;
			case 0x65:
				opcodeFX65(opcode, var_reg, &i_reg, memory, old);
				//running = false;
				break;
			case 0x0A:
				opcodeFX0A(opcode, &var_reg, &p_c, &keys);
				//running = false;
				break;
			case 0x1E:
				opcodeFX1E(opcode, var_reg, &i_reg);
				//running = false;
				break;
			default:
				printf("Opcode not recognised!\n");
				return EXIT_FAILURE;
			}
			break;
		default:
			printf("Opcode not recognised!\n");
			return EXIT_FAILURE;
		}
		
		if (loop_count >= 8) {
			if (delay_timer > 0)
				--delay_timer;
			if (sound_timer > 0)
				--sound_timer;
			loop_count = 0;
		}
		if (draw_flag == true) {
			buffer(pixels, display);
			render(pixels);
			draw_flag = false;
		}
		++loop_count;
	} 
	free(pixels);
	quit();
	return EXIT_SUCCESS;
}

