#ifndef CHIP_8_INSTRUCTIONS_H
#define CHIP_8_INSTRUCTIONS_H

#include "chip8_struct.h"

void opcode00E0(Chip8* chip8);							//Sets Display to 0's
void opcode00EE(Chip8* chip8, Stack* stack);					//Returns from address at top of stack
void opcode1NNN(Chip8* chip8);							//Sets p_c to NNN
void opcode2NNN(Chip8* chip8, Stack* stack);					//Pushes current p_c to stack and jumps to NNN
void opcode3XNN(Chip8* chip8);							//skips one instruction if vx == NN
void opcode4XNN(Chip8* chip8);							//skips one instruction if vx != NN
void opcode5XY0(Chip8* chip8);							//skips one instruction if vx == vy
void opcode6XNN(Chip8* chip8);  						//Sets register vx to NN
void opcode7XNN(Chip8* chip8);							//Adds NN to register vx
void opcode8XY0(Chip8* chip8);							//Sets vx to vy
void opcode8XY1(Chip8* chip8);							//Sets vx to vx OR vy
void opcode8XY2(Chip8* chip8);							//Sets vx to vx AND vy
void opcode8XY3(Chip8* chip8);							//Sets vx to vx ZOR vy
void opcode8XY4(Chip8* chip8);							//Sets vx to vx += vy if overflow var_reg set to 1 else 0
void opcode8XY5(Chip8* chip8);							//Sets vx to vx -= vy if underflow var_reg set to 0 else 1
void opcode8XY6(Chip8* chip8);							//Sets vf to vx >> 1
void opcode8XYE(Chip8* chip8);							//Sets vf to vx << 1
void opcode8XY7(Chip8* chip8);							//Sets vx to vy - vx var_reg16 set if vy >= vx
void opcode9XY0(Chip8* chip8);							//skips one instruction if vx != vy
void opcodeANNN(Chip8* chip8);							//Sets I_reg to NNN
void opcodeBNNN(Chip8* chip8);							//Jumps to address NNN plus value of V0
void opcodeCXNN(Chip8* chip8, int random_number);				//Sets vx to NN binaryAND random number
void opcodeDXYN(Chip8* chip8); 							//Display
void opcodeEX9E(Chip8* chip8);							//Skips one instruction if key[vx] is pressed
void opcodeEXA1(Chip8* chip8);							//Skips one instruction if key[vx] is not pressed
void opcodeFX07(Chip8* chip8);							//Sets var_reg[vx] to current value of delay_timer
void opcodeFX15(Chip8* chip8);							//Sets delay_timer to value of var_reg[vx]
void opcodeFX18(Chip8* chip8);							//Sets sound_timer to value of var_reg[vx]
void opcodeFX29(Chip8* chip8);
void opcodeFX33(Chip8* chip8);							//Binary coded decimal conversion
void opcodeFX55(Chip8* chip8);							//Loads var_reg into memory
void opcodeFX65(Chip8* chip8);							//Loads memory into v[0] to v[X]
void opcodeFX1E(Chip8* chip8);							//Adds var_reg[vx] to i_reg
void opcodeFX0A(Chip8* chip8);							//Gets key
#endif//CHIP_8_INSTRUCTIONS_H