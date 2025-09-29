#ifndef SDL_WINDOW
#define SDL_WINDOW

#define WIDTH 600
#define HEIGHT 400

#define TRUE 1
#define FALSE 0

typedef struct {

	char up,     up_justnow;
	char down,   down_justnow;
	char left,   left_justnow;
	char right,  right_justnow;

	char select, select_justnow;
	char cancel, cancel_justnow;

	char lmb,    lmb_justnow;
	int mouse_x, mouse_y;

} Input;

/*
 * sdl_window.c
 */
void draw_bordered_rect(int x, int y, int w, int h);

void draw_text(int x, int y, const char *string);
void draw_text_centered(int x, int y, const char *string);

/*
 * game.c
 */
void init_game(); // should take in game data file path (game data file has a "start state" entry so that the editor can test starting from arbitrary states)
void process_game(const Input *input);
void destroy_game();

/*
 * editor.c
 */
void init_editor(); // should take in game data file path
void process_editor(const Input *input);
void destroy_editor();

#endif