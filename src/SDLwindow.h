#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <stdbool.h>

typedef struct _Chip8 Chip8;
typedef struct _SDL SDL;
//SDL2 init function
bool init_sdl(SDL* sdl);
//SDL2 event function
void event_handler(SDL* sdl, Chip8* chip8);
//SDL2 buffer fucntion
void buffer(SDL* sdl, Chip8* chip8);
//SDL2 render function
void render(SDL* sdl, Chip8* chip8);
//Plays sound
void play_beep(SDL* sdl);
//SDL2 clean up function
void quit(SDL* sdl);



#endif//SDL_WINDOW_H
