#ifndef SDL_WINDOW
#define SDL_WINDOW

#define WIDTH 600
#define HEIGHT 400

#define TRUE 1
#define FALSE 0

typedef struct {

	char up,     up_edge; // "edge," as in "just changed"
	char down,   down_edge;
	char left,   left_edge;
	char right,  right_edge;

	char select, select_edge;
	char cancel, cancel_edge;

} Input;

/*
 * sdl_window.c
 */
void signal_start_game(const char *game_data_path); // used by menu.c
void signal_exit();

void draw_bordered_rect(int x, int y, int w, int h);

void draw_text(int x, int y, const char *string);
void draw_text_centered(int x, int y, const char *string);

/*
 * game.c
 */
int init_game(const char *game_data_path); // returns FALSE if failed to load from path (will automatically clean up after itself)
void process_game(const Input *input);
void destroy_game();

/*
 * menu.c
 */
void init_menu();
void process_menu(const Input *input);
void destroy_menu();
void report_corrupt_to_menu(); // when sdl_window.c tries to load a game, but fails, this is called to report that to the front-end

#endif