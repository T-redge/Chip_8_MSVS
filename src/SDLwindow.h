#include "chip8.h"

#include <SDL.h>
#include <SDL_mixer.h>

#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

//SDL2 init function
bool init();
//SDL2 event function
void event_handler(SDL_Event* event, bool* flag,uint8_t* keys);
//SDL2 buffer fucntion
void buffer(uint32_t* buffer, uint8_t display[][SCREEN_HEIGHT]);
//SDL2 render function
void render(uint32_t *pixels);
void play_beep();
//SDL2 clean up function
void quit();



#endif//SDL_WINDOW_H
