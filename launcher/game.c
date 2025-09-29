#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdl_window.h"

typedef struct {

	void (*process_state)(void *state, const Input *input); // all states start with these two functions (pseudo-inheritence through composition)
	void (*destroy_state)(void *state);

} State;

static State **states;
static unsigned int state_count;
static unsigned int current_state_index;



typedef struct {

	State base_state;

	unsigned int successor_index;
	char *text;

} DialogueState;

static void process_dialogue_state(void *state, const Input *input) {

	draw_bordered_rect(0, HEIGHT / 2, WIDTH, HEIGHT / 2);
	draw_text(10, 10 + HEIGHT / 2, ((DialogueState *) state)->text);

	if (input->select && input->select_edge) {
		current_state_index = ((DialogueState *) state)->successor_index;
	}
}

static void destroy_dialogue_state(void *state) {

	free(((DialogueState *) state)->text);
	free(state);
}

static State *create_dialogue_state(unsigned int successor_index, const char *text) {

	DialogueState *state = malloc(sizeof(DialogueState));

	state->base_state.process_state = process_dialogue_state;
	state->base_state.destroy_state = destroy_dialogue_state;

	state->successor_index = successor_index;

	state->text = malloc(sizeof(char) * (strlen(text) + 1));
	strcpy(state->text, text);

	return (State *) state;
}



void init_game(const char *game_data_path) {

	FILE *file = fopen(game_data_path, "r");

	if (!file) {
		printf("Could not open game file!\n");
		return;
	}

	// read start state and total state count
	fscanf(file, "%u %u", &current_state_index, &state_count);

	// allocate space for the states
	states = malloc(sizeof(State *) * state_count);

	// read all the states
	char flag[100] = {};
	unsigned int state_index = 0;

	while (fscanf(file, "%99s", flag) != EOF) {

		if (strcmp(flag, "#DIALOGUE") == 0) {

			unsigned int successor_index;
			char text[2048] = {};

			fscanf(file, "%u:\"%2047[^\"]\"", &successor_index, text);

			states[state_index++] = create_dialogue_state(successor_index, text);

		} else if (strcmp(flag, "#COMMENT") == 0) {

			fscanf(file, "\"%*[^\"]\"");

		}
	}

	// #CHOICE 2 2:"Eat bacon!" 3:"Eat pancakes!"

	fclose(file);
}

void process_game(const Input *input) {

	states[current_state_index]->process_state(states[current_state_index], input);
}

void destroy_game() {

	for (int i=0; i<state_count; i++)
		states[i]->destroy_state(states[i]);

	free(states);
}