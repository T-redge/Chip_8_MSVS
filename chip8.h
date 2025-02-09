#include "Stack.h"

#ifndef CHIP_8_INSTRUCTIONS_H
#define CHIP_8_INSTRUCTIONS_H

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


/******************************************/
/*Loading .ch8 file functions
/******************************************/
void load_rom(uint8_t* memory);
/******************************************/
/*Chip_8 Instructions
/******************************************/
uint16_t get_opcode(uint8_t* memory, uint16_t* p_c);									//Gets operator code from memory

void opcode00E0(uint8_t display[][SCREEN_HEIGHT], bool *draw_flag);									//Sets Display to 0's
void opcode00EE(uint16_t* p_c, Stack *stack);										//Returns from address at top of stack
void opcode1NNN(uint16_t opcode, uint16_t* p_c);									//Sets p_c to NNN
void opcode2NNN(uint16_t opcode, uint16_t* p_c, Stack *stack);								//Pushes current p_c to stack and jumps to NNN
void opcode3XNN(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg);							//skips one instruction if vx == NN
void opcode4XNN(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg);							//skips one instruction if vx != NN
void opcode5XY0(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg);							//skips one instruction if vx == vy
void opcode6XNN(uint16_t opcode, uint8_t* var_reg);  									//Sets register vx to NN
void opcode7XNN(uint16_t opcode, uint8_t* var_reg);									//Adds NN to register vx
void opcode8XY0(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vy
void opcode8XY1(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vx OR vy
void opcode8XY2(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vx AND vy
void opcode8XY3(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vx ZOR vy
void opcode8XY4(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vx += vy if overflow var_reg set to 1 else 0
void opcode8XY5(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vx -= vy if underflow var_reg set to 0 else 1
void opcode8XY6(uint16_t opcode, uint8_t* var_reg);									//Sets vf to vx >> 1
void opcode8XYE(uint16_t opcode, uint8_t* var_reg);									//Sets vf to vx << 1
void opcode8XY7(uint16_t opcode, uint8_t* var_reg);									//Sets vx to vy - vx var_reg16 set if vy >= vx
void opcode9XY0(uint16_t opcode, uint16_t* p_c, uint8_t* var_reg);							//skips one instruction if vx != vy
void opcodeANNN(uint16_t opcode, uint16_t* i_reg);									//Sets I_reg to NNN
void opcodeBNNN(uint16_t opcode, uint8_t* var_reg, uint16_t* p_c);							//Jumps to address NNN plus value of V0
void opcodeCXNN(uint16_t opcode, uint8_t* var_reg, int random_number);							//Sets vx to NN binaryAND random number
void opcodeDXYN(uint16_t opcode, uint8_t* var_reg, uint16_t i_reg, uint8_t display[][SCREEN_HEIGHT], uint8_t* memory, bool *draw_flag); 	//Display
void opcodeEX9E(uint16_t opcode, uint8_t* keys, uint16_t* p_c, uint8_t *var_reg);								//Skips one instruction if key[vx] is pressed
void opcodeEXA1(uint16_t opcode, uint8_t* keys, uint16_t* p_c, uint8_t* var_reg);								//Skips one instruction if key[vx] is not pressed
void opcodeFX07(uint16_t opcode, uint8_t* var_reg, uint8_t delay_timer);						//Sets var_reg[vx] to current value of delay_timer
void opcodeFX15(uint16_t opcode, uint8_t* var_reg, uint8_t *delay_timer);						//Sets delay_timer to value of var_reg[vx]
void opcodeFX18(uint16_t opcode, uint8_t* var_reg, uint8_t sound_timer);						//Sets sound_timer to value of var_reg[vx]
void opcodeFX29(uint16_t opcode, uint16_t* i_reg, uint8_t* var_reg, uint8_t* memory);
void opcodeFX33(uint16_t opcode, uint8_t* memory, uint16_t i_reg, uint8_t* var_reg);					//Binary coded decimal conversion
void opcodeFX55(uint16_t opcode, uint8_t* var_reg, uint16_t *i_reg, uint8_t* memory, bool old);				//Loads var_reg into memory
void opcodeFX65(uint16_t opcode, uint8_t* var_reg, uint16_t *i_reg, uint8_t* memory, bool old);				//Loads memory into v[0] to v[X]
void opcodeFX1E(uint16_t opcode, uint8_t* var_reg, uint16_t* i_reg);							//Adds var_reg[vx] to i_reg
void opcodeFX0A(uint16_t opcode, uint8_t* var_reg, uint16_t* p_c, uint8_t* keys);					//Gets key
#endif//CHIP_8_INSTRUCTIONS_H