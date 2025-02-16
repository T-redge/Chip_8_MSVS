#include "chip8_instruct.h"

#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H


#include <SDL.h>
#include <SDL_mixer.h>

//SDL2 init function
bool init();
//SDL2 event function
void event_handler(SDL_Event* event, bool* flag, Chip8* chip8);
//SDL2 buffer fucntion
void buffer(uint32_t* buffer, Chip8* chip8);
//SDL2 render function
void render(uint32_t *pixels);
//Plays sound
void play_beep();
//SDL2 clean up function
void quit();



#endif//SDL_WINDOW_H
