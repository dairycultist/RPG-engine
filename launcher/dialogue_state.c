#include <stdlib.h>
#include <string.h>
#include "sdl_window.h"
#include "state.h"

typedef struct {

	State base_state;

	unsigned int successor_index;
	char *text;

} DialogueState;

static int process_dialogue_state(void *state, const Input *input) {

	draw_bordered_rect(0, HEIGHT / 2, WIDTH, HEIGHT / 2);
	draw_text(10, 10 + HEIGHT / 2, ((DialogueState *) state)->text);

	if (input->select && input->select_edge) {
		return ((DialogueState *) state)->successor_index;
	} else {
		return -1;
	}
}

static void destroy_dialogue_state(void *state) {

	free(((DialogueState *) state)->text);
	free(state);
}

State *create_dialogue_state(unsigned int successor_index, const char *text) {

	DialogueState *state = malloc(sizeof(DialogueState));

	state->base_state.process_state = process_dialogue_state;
	state->base_state.destroy_state = destroy_dialogue_state;

	state->successor_index = successor_index;

	state->text = malloc(sizeof(char) * (strlen(text) + 1));
	strcpy(state->text, text);

	return (State *) state;
}