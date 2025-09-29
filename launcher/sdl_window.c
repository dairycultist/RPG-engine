#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "sdl_window.h"

static TTF_Font* font;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

typedef enum {

	MENU, GAME, PAUSED_ON_RESUME, PAUSED_ON_MENU

} WindowState;

static WindowState state;
static char running = TRUE;

int main() {

	printf("Opening RPG Engine...\n");

	// initialize a lot of static stuff
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0) {
		printf("Error initializing TTF:\n%s\n", TTF_GetError());
		return 1;
	}

	font = TTF_OpenFont("VCR_MONO.ttf", 21);

	if (!font) {
		printf("Error loading engine font 'VCR_MONO.ttf'.");
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

	// initialize startup state
	state = MENU;
	init_menu();

	// process events until window is closed
	SDL_Event event;
	SDL_Rect letterbox = { 0, 0, WIDTH * 2, HEIGHT * 2 };

	Input input;

	while (running) {

		input.up_edge     = FALSE;
		input.down_edge   = FALSE;
		input.left_edge   = FALSE;
		input.right_edge  = FALSE;
		input.select_edge = FALSE;
		input.cancel_edge = FALSE;

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
					case SDL_SCANCODE_C:

						if (event.key.state == SDL_PRESSED) {

							switch (state) {
								case MENU: break;
								case GAME: state = PAUSED_ON_RESUME; break;
								case PAUSED_ON_RESUME:
								case PAUSED_ON_MENU: state = GAME; break;
							}
						}
						break;

					case SDL_SCANCODE_UP:

						switch (state) {
							case MENU:
							case GAME:
								input.up      = event.key.state == SDL_PRESSED;
								input.up_edge = TRUE;
								break;
							case PAUSED_ON_RESUME: if (event.key.state == SDL_PRESSED) { state = PAUSED_ON_MENU; } break;
							case PAUSED_ON_MENU: if (event.key.state == SDL_PRESSED) { state = PAUSED_ON_RESUME; } break;
						}
						break;

					case SDL_SCANCODE_DOWN:

						switch (state) {
							case MENU:
							case GAME:
								input.down      = event.key.state == SDL_PRESSED;
								input.down_edge = TRUE;
								break;
							case PAUSED_ON_RESUME: if (event.key.state == SDL_PRESSED) { state = PAUSED_ON_MENU; } break;
							case PAUSED_ON_MENU: if (event.key.state == SDL_PRESSED) { state = PAUSED_ON_RESUME; } break;
						}
						break;

					case SDL_SCANCODE_RETURN:
					case SDL_SCANCODE_Z:

						switch (state) {
							case MENU:
							case GAME:
								input.select      = event.key.state == SDL_PRESSED;
								input.select_edge = TRUE;
								break;
							case PAUSED_ON_RESUME: state = GAME; break;
							case PAUSED_ON_MENU: destroy_game(); state = MENU; init_menu(); break;
						}
						break;

					default: break;
				}
			}
		}

		// set render target to screen_buffer
		SDL_SetRenderTarget(renderer, screen_buffer);
		
		// logic/rendering to screen_buffer
		if (state == MENU || state == GAME) {

			// clear screen_buffer to grey
			SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
			SDL_RenderClear(renderer);

			// process appropriate state
			state == MENU ? process_menu(&input) : process_game(&input);

		} else {

			// render the pause menu over the game
			draw_bordered_rect(WIDTH / 2 - 150, HEIGHT / 2 - 90, 300, 180);
			draw_text_centered(WIDTH / 2, HEIGHT / 2 - 60, "Paused");

			draw_text_centered(WIDTH / 2, HEIGHT / 2, state == PAUSED_ON_RESUME ? "[Resume]" : "Resume");
			draw_text_centered(WIDTH / 2, HEIGHT / 2 + 40, state == PAUSED_ON_MENU   ? "[Return to Menu]"   : "Return to Menu");
		}
		
		SDL_SetRenderTarget(renderer, NULL); 						// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 	// render screen_buffer
		SDL_RenderPresent(renderer); 								// present rendered content to screen

		SDL_Delay(1000 / 60);
	}

	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void signal_start_game(const char *game_data_path) {

	destroy_menu();
	state = GAME;
	init_game(game_data_path);
}

void signal_exit() {

	running = FALSE;
}

void draw_bordered_rect(int x, int y, int w, int h) {

	SDL_Rect rect = { x, y, w, h };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(renderer, &rect);
	rect.x++;
	rect.y++;
	rect.w -= 2;
	rect.h -= 2;
	SDL_RenderDrawRect(renderer, &rect);
}

void draw_text(int x, int y, const char *string) {

	static SDL_Color white = { 255, 255, 255 };

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, string, white); // TTF_RenderText_Blended for AA
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect text_rect = { x, y };
	TTF_SizeText(font, string, &text_rect.w, &text_rect.h);

	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void draw_text_centered(int x, int y, const char *string) {

	int w;
	TTF_SizeText(font, string, &w, NULL);

	draw_text(x - (w / 2), y, string);
}