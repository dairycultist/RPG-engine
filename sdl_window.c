
// gcc sdl_window.c -lSDL2_image -lSDL2_ttf $(sdl2-config --cflags) $(sdl2-config --libs)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

#define WIDTH 600
#define HEIGHT 400

#define TRUE 1
#define FALSE 0

void init_editor() {

}

void process_editor() {
	
}

void init_game() {

}

void process_game() {

}

typedef enum {

	GAME, GAME_PAUSED_ON_RESUME, GAME_PAUSED_ON_EDIT, GAME_PAUSED_ON_QUIT, EDITOR

} WindowState;

static WindowState state = GAME;

int main() {

	printf("Opening RPG Engine...\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0) {
		printf("Error initializing TTF:\n%s\n", TTF_GetError());
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("OpenDyslexic.ttf", 20);
	// TTF_CloseFont(font);

	if (!font) {
		printf("Error loading engine font 'OpenDyslexic.ttf'.");
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

	// pre-render pause menu text
	SDL_Color white = {255, 255, 255};

	SDL_Surface* text_surface = TTF_RenderText_Blended(font, "paused", white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect text_rect;
	text_rect.x = 20;
	text_rect.y = 20;

	TTF_SizeText(font, "paused", &text_rect.w, &text_rect.h);

	// process events until window is closed
	SDL_Event event;
	SDL_Rect letterbox = {0, 0, WIDTH * 2, HEIGHT * 2};

	char running = TRUE;

	char key_up = FALSE; // will switch to a struct containing all the key info (directions + select + back, with both pressed and just pressed)

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = FALSE;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				#define MIN(a, b) ((a) > (b) ? (b) : (a))

				// dynamically change letterbox based on screen resize
				letterbox.w = MIN(event.window.data1, event.window.data2 * WIDTH / HEIGHT);
				letterbox.h = MIN(event.window.data2, event.window.data1 * HEIGHT / WIDTH);

				letterbox.x = (event.window.data1 - letterbox.w) / 2;
				letterbox.y = (event.window.data2 - letterbox.h) / 2;

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				switch (event.key.keysym.scancode) {

					case SDL_SCANCODE_ESCAPE:

						if (event.key.state == SDL_PRESSED) {
							if (state == GAME) {
								state = GAME_PAUSED_ON_RESUME;
							} else if (state == GAME_PAUSED_ON_RESUME || state == GAME_PAUSED_ON_EDIT || state == GAME_PAUSED_ON_QUIT) {
								state = GAME;
							}
						}
						break;

					case SDL_SCANCODE_UP: key_up = event.key.state == SDL_PRESSED; break;

					default: break;
				}
			}
		}
		
		// logic/rendering to screen_buffer
		if (state == EDITOR || state == GAME) {

			SDL_SetRenderTarget(renderer, screen_buffer); 		// set render target to screen_buffer
			SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); 	// clear screen_buffer to grey
			SDL_RenderClear(renderer);

			if (state == GAME)
				process_game();
			else
				process_editor();

		} else {

			// render the pause menu over the game
		}
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
		
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