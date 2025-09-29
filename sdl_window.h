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

} Input;

/*
 * sdl_window.c
 */
void start_game(const char *game_data_path); // used by menu.c

void draw_bordered_rect(int x, int y, int w, int h);

void draw_text(int x, int y, const char *string);
void draw_text_centered(int x, int y, const char *string);

/*
 * game.c
 */
void init_game(const char *game_data_path);
void process_game(const Input *input);
void destroy_game();

/*
 * menu.c
 */
void init_menu();
void process_menu(const Input *input);
void destroy_menu();

#endif