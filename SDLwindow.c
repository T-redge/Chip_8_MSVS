#include "SDLwindow.h"

int VIDEO_WIDTH		 = 64;
int VIDEO_HEIGHT	 = 32;
const int VIDEO_SCALE	 = 20;

SDL_Window* window	= NULL;
SDL_Texture* texture	= NULL;
SDL_Renderer* renderer	= NULL;

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL Initialization failed. SDL Error: %s\n", SDL_GetError());
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

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
	if (texture == NULL) {
		printf("Texture could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}
void event_handler(SDL_Event* event, bool* flag)
{
	while (SDL_PollEvent(event) != 0) {
		if (event->type == SDL_QUIT) {
			*flag = false;
		}
	}
}
void render(uint8_t display[][SCREEN_HEIGHT], uint32_t *pixels, int pitch)
{
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	SDL_memset(pixels, 255, (uint64_t)pitch * 32);

	for (int y = 0; y < 32; ++y) {
		for (int x = 0; x < 64; ++x) {
			if (display[x][y] != 0) {
				pixels[y * 64 + x] = 0;
			}
		}
	}

	SDL_UnlockTexture(texture);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void quit()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	window = NULL;
	renderer = NULL;
	texture = NULL;

	SDL_Quit();
	printf("SDL has closed succesfully!\n");
}