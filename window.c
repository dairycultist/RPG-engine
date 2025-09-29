#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

#define WIDTH 300
#define HEIGHT 300
#define ASPECT_RATIO (WIDTH/HEIGHT)

#define TRUE 1
#define FALSE 0

void init() {

}

void process() {
	
}

int main() {

	printf("Opening RPG Engine...\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("RPG Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2, HEIGHT * 2, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

	init();

	// process events until window is closed
	SDL_Event event;
	SDL_Rect letterbox = {0, 0, WIDTH * 2, HEIGHT * 2};

	char running = TRUE;

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = FALSE;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				#define MIN(a, b) ((a) > (b) ? (b) : (a))

				// dynamically change letterbox based on screen resize
				letterbox.w = MIN(event.window.data1, event.window.data2 * ASPECT_RATIO);
				letterbox.h = MIN(event.window.data2, event.window.data1 / ASPECT_RATIO);

				letterbox.x = (event.window.data1 - letterbox.w) / 2;
				letterbox.y = (event.window.data2 - letterbox.h) / 2;

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				printf("%d %d\n", event.key.keysym.scancode, event.key.state == SDL_PRESSED);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 			// clear window to black
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 				// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); 			// clear screen_buffer to grey
		SDL_RenderClear(renderer);
		process();		 											// let the programmer do logic/render stuff to screen_buffer
		SDL_SetRenderTarget(renderer, NULL); 						// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 	// render screen_buffer
		SDL_RenderPresent(renderer); 								// present rendered content to screen

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}