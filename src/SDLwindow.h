#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include "chip8_instruct.h"
#include <SDL.h>
#include <SDL_mixer.h>

typedef struct SDL {
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	Mix_Chunk* beep;

	SDL_Event event;

	uint32_t* pixels;

	int video_width;
	int video_height;
	int video_scale;

	bool running_flag;
}SDL;

//SDL2 init function
bool init_sdl(SDL* sdl);
//SDL2 event function
void event_handler(SDL* sdl, Chip8* chip8);
//SDL2 buffer fucntion
void buffer(SDL* sdl, Chip8* chip8);
//SDL2 render function
void render(SDL* sdl);
//Plays sound
void play_beep(SDL* sdl);
//SDL2 clean up function
void quit(SDL* sdl);



#endif//SDL_WINDOW_H
