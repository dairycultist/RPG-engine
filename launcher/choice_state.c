#include <stdlib.h>
#include <string.h>
#include "sdl_window.h"
#include "state.h"

typedef struct {

	State base_state;

	char *prompt;
	int choice_count;
	char **choice_texts;
	int *choice_successors;

} ChoiceState;

static int selected_choice = 0;

static int process_choice_state(void *state, const Input *input) {

	draw_bordered_rect(0, HEIGHT / 2, WIDTH, HEIGHT / 2);
	draw_text(10, 10 + HEIGHT / 2, ((ChoiceState *) state)->prompt);

	// TODO display all choices (currently just selected cuz ez)
	draw_text(10, 50 + HEIGHT / 2, ((ChoiceState *) state)->choice_texts[selected_choice]);

	if (input->select && input->select_edge) {

		int successor = ((ChoiceState *) state)->choice_successors[selected_choice];
		selected_choice = 0;
		return successor;

	} else {

		if (input->up && input->up_edge && --selected_choice == -1)
			selected_choice += ((ChoiceState *) state)->choice_count;

		if (input->down && input->down_edge && ++selected_choice == ((ChoiceState *) state)->choice_count)
			selected_choice = 0;

		return -1;
	}
}

static void destroy_choice_state(void *state) {

	for (int i=0; i<((ChoiceState *) state)->choice_count; i++)
		free(((ChoiceState *) state)->choice_texts[i]);

	free(((ChoiceState *) state)->prompt);
	free(((ChoiceState *) state)->choice_texts);
	free(((ChoiceState *) state)->choice_successors);

	free(state);
}

State *create_choice_state(char *prompt, int choice_count, char **choice_texts, int *choice_successors) {

	ChoiceState *state = malloc(sizeof(ChoiceState));

	state->base_state.process_state = process_choice_state;
	state->base_state.destroy_state = destroy_choice_state;

	state->prompt = prompt;
	state->choice_count = choice_count;
	state->choice_texts = choice_texts;
	state->choice_successors = choice_successors;

	return (State *) state;
}