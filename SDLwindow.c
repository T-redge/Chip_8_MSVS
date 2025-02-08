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
				keys[0x0] = 1;
			case SDL_SCANCODE_2:
				keys[0x1] = 1;
			case SDL_SCANCODE_3:
				keys[0x2] = 1;
			case SDL_SCANCODE_4:
				keys[0x3] = 1;
			case SDL_SCANCODE_Q:
				keys[0x4] = 1;
			case SDL_SCANCODE_W:
				keys[0x5] = 1;
			case SDL_SCANCODE_E:
				keys[0x6] = 1;
			case SDL_SCANCODE_R:
				keys[0x7] = 1;
			case SDL_SCANCODE_A:
				keys[0x8] = 1;
			case SDL_SCANCODE_S:
				keys[0x9] = 1;
			case SDL_SCANCODE_D:
				keys[0xA] = 1;
			case SDL_SCANCODE_F:
				keys[0xB] = 1;
			case SDL_SCANCODE_Z:
				keys[0xC] = 1;
			case SDL_SCANCODE_X:
				keys[0xD] = 1;
			case SDL_SCANCODE_C:
				keys[0xE] = 1;
			case SDL_SCANCODE_V:
				keys[0xF] = 1;
			default:
				printf("Scancode not recognised!\n");
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event->key.keysym.scancode) {
			case SDL_SCANCODE_1:
				keys[0x0] = 0;
			case SDL_SCANCODE_2:
				keys[0x1] = 0;
			case SDL_SCANCODE_3:
				keys[0x2] = 0;
			case SDL_SCANCODE_4:
				keys[0x3] = 0;
			case SDL_SCANCODE_Q:
				keys[0x4] = 0;
			case SDL_SCANCODE_W:
				keys[0x5] = 0;
			case SDL_SCANCODE_E:
				keys[0x6] = 0;
			case SDL_SCANCODE_R:
				keys[0x7] = 0;
			case SDL_SCANCODE_A:
				keys[0x8] = 0;
			case SDL_SCANCODE_S:
				keys[0x9] = 0;
			case SDL_SCANCODE_D:
				keys[0xA] = 0;
			case SDL_SCANCODE_F:
				keys[0xB] = 0;
			case SDL_SCANCODE_Z:
				keys[0xC] = 0;
			case SDL_SCANCODE_X:
				keys[0xD] = 0;
			case SDL_SCANCODE_C:
				keys[0xE] = 0;
			case SDL_SCANCODE_V:
				keys[0xF] = 0;
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
void render(uint8_t display[][SCREEN_HEIGHT], uint32_t *pixels, int pitch)
{
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	SDL_memset(pixels, 255, (uint64_t)pitch * 32);

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			pixels[(y * 64) + x] = (0xFFFFFF00 * display[x][y]) | 0x000000FF;
			
		
			
		}
	}

	SDL_UnlockTexture(texture);
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