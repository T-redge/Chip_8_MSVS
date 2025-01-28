#include "chip8.h"

#include <SDL.h>

#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

//SDL2 init function
bool init();
//SDL2 event function
void event_handler(SDL_Event* event, bool* flag);
//SDL2 render function
void render(uint8_t display[][SCREEN_HEIGHT], uint32_t *pixels, int pitch);
//SDL2 clean up function
void quit();



#endif//SDL_WINDOW_H
