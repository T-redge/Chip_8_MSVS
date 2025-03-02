/*
*	CHIP_8 Interpretor + Rendering Capabilities
*/
#include <time.h>
#include "SDLwindow.h"

int main(int argc, char* argv[])
{
	if (argc != 0);
	if (argv[0]);
	/********************************************************************************/
	/*		                 Initialisation					*/
	/********************************************************************************/
	Stack stack;
	init_stack(&stack);

	Chip8 chip8;
	init_chip8(&chip8);
	load_rom(&chip8);

	SDL sdl;
	init_sdl(&sdl);

	//Init Rand
	srand((unsigned)time(NULL));
	int rand_var = (rand() % (255 - 0 + 1)) + 0;
  
/********************************************************************************/
/*		                 Program Loop				                                     	*/
/********************************************************************************/
	
  while (sdl.running_flag) {
		//Fetch
		chip8.opcode = get_opcode(&chip8);

		//Decode + Execute
		switch (chip8.opcode & 0xF000) {
			case 0x0000:
				switch (chip8.opcode & 0xFF) {
				case 0xE0:
					opcode00E0(&chip8);
					break;
				case 0xEE:
					opcode00EE(&chip8, &stack);
					break;
				default:
					return EXIT_FAILURE;
				}
				break;
			case 0x1000:
				opcode1NNN(&chip8);
				break;
			case 0x2000:
				opcode2NNN(&chip8, &stack);
				break;
			case 0x3000:
				opcode3XNN(&chip8);
				break;
			case 0x4000:
				opcode4XNN(&chip8);
				break;
			case 0x5000:
				opcode5XY0(&chip8);
				break;
			case 0x6000:
				opcode6XNN(&chip8);
				break;
			case 0x7000:
				opcode7XNN(&chip8);
				break;
			case 0x8000:
				switch (chip8.opcode & 0x000F) {
				case 0x0:
					opcode8XY0(&chip8);
					break;
				case 0x1:
					opcode8XY1(&chip8);
					break;
				case 0x2:
					opcode8XY2(&chip8);
					break;
				case 0x3:
					opcode8XY3(&chip8);
					break;
				case 0x4:
					opcode8XY4(&chip8);
					break;
				case 0x5:
					opcode8XY5(&chip8);
					break;
				case 0x6:
					opcode8XY6(&chip8);
					break;
				case 0x7:
					opcode8XY7(&chip8);
					break;
				case 0xE:
					opcode8XYE(&chip8);
					break;
				default:
					printf("Opcode not recognised!\n");
					return EXIT_FAILURE;
				}
				break;
			case 0x9000:
				opcode9XY0(&chip8);
				break;
			case 0xA000:
				opcodeANNN(&chip8);
				break;
			case 0xB000:
				opcodeBNNN(&chip8);
				break;
			case 0xC000:
				opcodeCXNN(&chip8, rand_var);
				break;
			case 0xD000:
				opcodeDXYN(&chip8);
				break;
			case 0xE000:
				switch (chip8.opcode & 0xFF) {
				case 0x9E:
					opcodeEX9E(&chip8);
					break;
				case 0xA1:
					opcodeEXA1(&chip8);
					break;
				default:
					printf("Opcode not recognised!\n");
					return EXIT_FAILURE;
				}
				break;
			case 0xF000:
				switch (chip8.opcode & 0xFF) {
				case 0x07:
					opcodeFX07(&chip8);
					//running = false;
					break;
				case 0x15:
					opcodeFX15(&chip8);
					//running = false;
					break;
				case 0x18:
					opcodeFX18(&chip8);
					break;
				case 0x29:
					opcodeFX29(&chip8);
					//running = false;
					break;
				case 0x33:
					opcodeFX33(&chip8);
					//running = false;
					break;
				case 0x55:
					opcodeFX55(&chip8);
					//running = false;
					break;
				case 0x65:
					opcodeFX65(&chip8);
					//running = false;
					break;
				case 0x0A:
					opcodeFX0A(&chip8);
					//running = false;
					break;
				case 0x1E:
					opcodeFX1E(&chip8);
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
		++chip8.loop_counter;

		//Event Handling
		event_handler(&sdl, &chip8);

		//Drawing to screen
		if (chip8.draw_flag == true) {
			buffer(&sdl, &chip8);
			render(&sdl);
			chip8.draw_flag = false;
		}

		//Updating timers
		if (chip8.loop_counter == 8) {
			update_timers(&chip8);
			chip8.loop_counter = 0;
		}
		//Delay to slow executing loop
		SDL_Delay(1);
	} 
	free(sdl.pixels);
	quit(&sdl);
	return EXIT_SUCCESS;
}

