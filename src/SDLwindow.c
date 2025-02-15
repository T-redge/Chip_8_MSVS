#include "SDLwindow.h"

int VIDEO_WIDTH		 = 64;
int VIDEO_HEIGHT	 = 32;
const int VIDEO_SCALE	 = 20;

SDL_Window* window	= NULL;
SDL_Texture* texture	= NULL;
SDL_Renderer* renderer	= NULL;
Mix_Chunk* beep		= NULL;

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL Initialization failed. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_Mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError());
		return false;
	}

	window = SDL_CreateWindow("Chip_8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH * VIDEO_SCALE, VIDEO_HEIGHT * VIDEO_SCALE, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, VIDEO_WIDTH, VIDEO_HEIGHT);
	if (texture == NULL) {
		printf("Texture could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	beep = Mix_LoadWAV("src/sound/short_beep.wav");
	if (beep == NULL) {
		printf("Beep could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}
void event_handler(SDL_Event* event, bool* flag, uint8_t* keys)
{
	while (SDL_PollEvent(event) != 0) {
		switch (event->type) {
		case SDL_QUIT:
			*flag = false;
			break;
		case SDL_KEYDOWN:
			switch (event->key.keysym.scancode) {
			case SDL_SCANCODE_1:
				keys[0x1] = 1;
				break;
			case SDL_SCANCODE_2:
				keys[0x2] = 1;
				break;
			case SDL_SCANCODE_3:
				keys[0x3] = 1;
				break;
			case SDL_SCANCODE_4:
				keys[0xC] = 1;
				break;
			case SDL_SCANCODE_Q:
				keys[0x4] = 1;
				break;
			case SDL_SCANCODE_W:
				keys[0x5] = 1;
				break;
			case SDL_SCANCODE_E:
				keys[0x6] = 1;
				break;
			case SDL_SCANCODE_R:
				keys[0xD] = 1;
				break;
			case SDL_SCANCODE_A:
				keys[0x7] = 1;
				break;
			case SDL_SCANCODE_S:
				keys[0x8] = 1;
				break;
			case SDL_SCANCODE_D:
				keys[0x9] = 1;
				break;
			case SDL_SCANCODE_F:
				keys[0xE] = 1;
				break;
			case SDL_SCANCODE_Z:
				keys[0xA] = 1;
				break;
			case SDL_SCANCODE_X:
				keys[0x0] = 1;
				break;
			case SDL_SCANCODE_C:
				keys[0xB] = 1;
				break;
			case SDL_SCANCODE_V:
				keys[0xF] = 1;
				break;
			default:
				printf("Scancode not recognised!\n");
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event->key.keysym.scancode) {
			case SDL_SCANCODE_1:
				keys[0x1] = 0;
				break;
			case SDL_SCANCODE_2:
				keys[0x2] = 0;
				break;
			case SDL_SCANCODE_3:
				keys[0x3] = 0;
				break;
			case SDL_SCANCODE_4:
				keys[0xC] = 0;
				break;
			case SDL_SCANCODE_Q:
				keys[0x4] = 0;
				break;
			case SDL_SCANCODE_W:
				keys[0x5] = 0;
				break;
			case SDL_SCANCODE_E:
				keys[0x6] = 0;
				break;
			case SDL_SCANCODE_R:
				keys[0xD] = 0;
				break;
			case SDL_SCANCODE_A:
				keys[0x7] = 0;
				break;
			case SDL_SCANCODE_S:
				keys[0x8] = 0;
				break;
			case SDL_SCANCODE_D:
				keys[0x9] = 0;
				break;
			case SDL_SCANCODE_F:
				keys[0xE] = 0;
				break;
			case SDL_SCANCODE_Z:
				keys[0xA] = 0;
				break;
			case SDL_SCANCODE_X:
				keys[0x0] = 0;
				break;
			case SDL_SCANCODE_C:
				keys[0xB] = 0;
				break;
			case SDL_SCANCODE_V:
				keys[0xF] = 0;
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
void buffer(uint32_t* buffer, uint8_t display[][SCREEN_HEIGHT])
{
	for (int y = 0; y < SCREEN_HEIGHT; y++) 
		for (int x = 0; x < SCREEN_WIDTH; x++)
			buffer[(y * SCREEN_WIDTH) + x] = (0xFFFFFF00 * display[x][y]) | 0x000000FF;
}
void render(uint32_t *pixels)
{
	SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void play_beep()
{
	Mix_PlayChannel(-1, beep, 0);
}
void quit()
{
	Mix_FreeChunk(beep);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	beep = NULL;
	window = NULL;
	renderer = NULL;
	texture = NULL;

	SDL_Quit();
	Mix_Quit();
	printf("SDL has closed succesfully!\n");
}