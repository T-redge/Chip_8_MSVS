#include "SDLwindow.h"
#include "chip8_struct.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_mixer.h>

struct _SDL {
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
};
struct _Chip8
{
	uint8_t memory[MEMORY_SIZE];
	uint8_t display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
	uint8_t var_reg[REGISTER_SIZE];
	uint8_t keys[KEYS_SIZE];
	uint8_t delay_timer;
	uint8_t sound_timer;

	uint16_t p_c;
	uint16_t i_reg;
	uint16_t opcode;

	int loop_counter;

	bool old_instruction;
	bool draw_flag;
};

bool init_sdl(SDL* sdl)
{
	sdl->window = NULL;
	sdl->texture = NULL;
	sdl->renderer = NULL;
	sdl->beep = NULL;

	sdl->pixels = malloc(
		(DISPLAY_HEIGHT * DISPLAY_WIDTH) * sizeof(uint32_t));

	sdl->video_width = 64;
	sdl->video_height = 32;
	sdl->video_scale = 20;

	sdl->running_flag = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL Initialization failed. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_Mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError());
		return false;
	}

	sdl->window = SDL_CreateWindow("Chip_8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl->video_width * sdl->video_scale, sdl->video_height * sdl->video_scale, SDL_WINDOW_SHOWN);
	if (sdl->window == NULL) {
		printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
	if (sdl->renderer == NULL) {
		printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sdl->video_width, sdl->video_height);
	if (sdl->texture == NULL) {
		printf("Texture could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	sdl->beep = Mix_LoadWAV("src/sound/short_beep.wav");
	if (sdl->beep == NULL) {
		printf("Beep could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}
void event_handler(SDL* sdl, Chip8* chip8)
{
	while (SDL_PollEvent(&sdl->event) != 0) {
		switch (sdl->event.type) {
		case SDL_QUIT:
			sdl->running_flag = false;
			break;
		case SDL_KEYDOWN:
			switch (sdl->event.key.keysym.scancode) {
			case SDL_SCANCODE_1:
				chip8->keys[0x1] = 1;
				break;
			case SDL_SCANCODE_2:
				chip8->keys[0x2] = 1;
				break;
			case SDL_SCANCODE_3:
				chip8->keys[0x3] = 1;
				break;
			case SDL_SCANCODE_4:
				chip8->keys[0xC] = 1;
				break;
			case SDL_SCANCODE_Q:
				chip8->keys[0x4] = 1;
				break;
			case SDL_SCANCODE_W:
				chip8->keys[0x5] = 1;
				break;
			case SDL_SCANCODE_E:
				chip8->keys[0x6] = 1;
				break;
			case SDL_SCANCODE_R:
				chip8->keys[0xD] = 1;
				break;
			case SDL_SCANCODE_A:
				chip8->keys[0x7] = 1;
				break;
			case SDL_SCANCODE_S:
				chip8->keys[0x8] = 1;
				break;
			case SDL_SCANCODE_D:
				chip8->keys[0x9] = 1;
				break;
			case SDL_SCANCODE_F:
				chip8->keys[0xE] = 1;
				break;
			case SDL_SCANCODE_Z:
				chip8->keys[0xA] = 1;
				break;
			case SDL_SCANCODE_X:
				chip8->keys[0x0] = 1;
				break;
			case SDL_SCANCODE_C:
				chip8->keys[0xB] = 1;
				break;
			case SDL_SCANCODE_V:
				chip8->keys[0xF] = 1;
				break;
			default:
				printf("Scancode not recognised!\n");
				break;
			}
			break;
		case SDL_KEYUP:
			switch (sdl->event.key.keysym.scancode) {
			case SDL_SCANCODE_1:
				chip8->keys[0x1] = 0;
				break;
			case SDL_SCANCODE_2:
				chip8->keys[0x2] = 0;
				break;
			case SDL_SCANCODE_3:
				chip8->keys[0x3] = 0;
				break;
			case SDL_SCANCODE_4:
				chip8->keys[0xC] = 0;
				break;
			case SDL_SCANCODE_Q:
				chip8->keys[0x4] = 0;
				break;
			case SDL_SCANCODE_W:
				chip8->keys[0x5] = 0;
				break;
			case SDL_SCANCODE_E:
				chip8->keys[0x6] = 0;
				break;
			case SDL_SCANCODE_R:
				chip8->keys[0xD] = 0;
				break;
			case SDL_SCANCODE_A:
				chip8->keys[0x7] = 0;
				break;
			case SDL_SCANCODE_S:
				chip8->keys[0x8] = 0;
				break;
			case SDL_SCANCODE_D:
				chip8->keys[0x9] = 0;
				break;
			case SDL_SCANCODE_F:
				chip8->keys[0xE] = 0;
				break;
			case SDL_SCANCODE_Z:
				chip8->keys[0xA] = 0;
				break;
			case SDL_SCANCODE_X:
				chip8->keys[0x0] = 0;
				break;
			case SDL_SCANCODE_C:
				chip8->keys[0xB] = 0;
				break;
			case SDL_SCANCODE_V:
				chip8->keys[0xF] = 0;
				break;
			default:
				printf("Scancode not recognised!\n");
				break;
			}
			break;
		default:
			break;
		}
	}
}
void buffer(SDL* sdl, Chip8* chip8)
{
	for (int y = 0; y < DISPLAY_HEIGHT; y++)
		for (int x = 0; x < DISPLAY_WIDTH; x++)
			sdl->pixels[(y * DISPLAY_WIDTH) + x] = (0xFFFFFF00 * chip8->display[x][y]) | 0x000000FF;
}
void render(SDL* sdl, Chip8* chip8)
{
	if (chip8->draw_flag == true) {
		buffer(sdl, chip8);
		SDL_UpdateTexture(sdl->texture, NULL, sdl->pixels, DISPLAY_WIDTH * sizeof(uint32_t));
		SDL_RenderClear(sdl->renderer);
		SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
		SDL_RenderPresent(sdl->renderer);
		chip8->draw_flag = false;
	}
}
void play_beep(SDL* sdl)
{
	Mix_PlayChannel(-1, sdl->beep, 0);
}
void quit(SDL* sdl)
{
	free(sdl->pixels);

	Mix_FreeChunk(sdl->beep);
	SDL_DestroyTexture(sdl->texture);
	SDL_DestroyRenderer(sdl->renderer);
	SDL_DestroyWindow(sdl->window);

	sdl->beep = NULL;
	sdl->window = NULL;
	sdl->renderer = NULL;
	sdl->texture = NULL;

	SDL_Quit();
	Mix_Quit();
	printf("SDL has closed succesfully!\n");
}