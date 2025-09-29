#ifndef SDL_WINDOW
#define SDL_WINDOW

#define WIDTH 600
#define HEIGHT 400

#define TRUE 1
#define FALSE 0

/*
 * sdl_window.c
 */
void draw_bordered_rect(int x, int y, int w, int h);

void draw_text(int x, int y, const char *string);
void draw_text_centered(int x, int y, const char *string);

/*
 * game.c
 */
void init_game();    // should take in game data file path
void process_game(); // should take in input struct
void destroy_game();

/*
 * editor.c
 */
void init_editor();
void process_editor();
void destroy_editor();

#endif